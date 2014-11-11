#!/usr/bin/env python
#
# Copyright 2007 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import os
import re
import urllib
import webapp2
import jinja2
import logging

from datamodel import *
from datahandle import *
from google.appengine.ext import blobstore
from google.appengine.ext.webapp import blobstore_handlers
from google.appengine.api import images
from google.appengine.ext import ndb

import utils

template_dir = os.path.join(os.path.dirname(__file__), "new_templates")
jinja_env = jinja2.Environment(loader = jinja2.FileSystemLoader(template_dir), autoescape = True)
jinja_env.globals['get_serving_url'] = images.get_serving_url
jinja_env.globals['len'] = len
jinja_env.globals['eval'] = eval

class PageHandler(webapp2.RequestHandler):
	def get(self):
		self.redirect('/default')

	def post(self):
		self.redirect('/default')

	def write(self, *a, **kw):
		self.response.out.write(*a, **kw)

	def render_str(self, template, **params):
		t = jinja_env.get_template(template)
		return t.render(params)

	def render(self, template, **kw):
		self.write(self.render_str(template, **kw))

	def page_error(self):
		self.redirect("/")

	def set_secure_cookie(self, name, val):
		secureVal = utils.secure_cookie_val(name, val)
		self.response.headers.add_header('Set-Cookie', "%s=%s" % (name, str(secureVal)))

	def read_secure_cookie(self, name):
		secureVal = self.request.cookies.get(name)
		if secureVal and utils.check_secure_cookie_val(name, secureVal):
			return secureVal.split('|')[0]

	def del_cookie(self, name):
		self.response.headers.add_header('Set-Cookie', "%s=" % name)

	def login(self, user):
		self.set_secure_cookie('userinfo', "id=%s" % user.key.id())

	def logout(self):
		logging.info("in logout proc")
		self.del_cookie('userinfo')

	def initialize(self, *a, **kw):
		webapp2.RequestHandler.initialize(self, *a, **kw)
		self.user = self.user_cookie_authenticate()

	def user_cookie_authenticate(self):
		userinfo = utils.format_cookie(self.read_secure_cookie('userinfo'))
		userid = userinfo.get('id')
		if userid:
			return User.get_by_id(userid)

class MainHandler(PageHandler):
	def get(self):
		if not self.user:
			templateVals = {'me': ""}
			qry = Picture.query()
			qry1 = qry.order(-Picture.viewed)
			most_viewed = qry1.fetch(100)
			templateVals['most_viewed'] = most_viewed
			self.render('index.html', **templateVals)
		else:
			templateVals = {'me': self.user}
			qry = Picture.query()
			qry1 = qry.order(-Picture.viewed)
			most_viewed = qry1.fetch(100)
			templateVals['most_viewed'] = most_viewed
			self.render('index_user.html', **templateVals)

class UserHomeHandler(PageHandler):
	def get(self):
		#userid = str(urllib.unquote(resource))
		if self.user:
			templateVals = {'me': self.user}
			self.render('user_home.html', **templateVals)
		else:
			self.redirect('/')

class UserSettingsHandler(blobstore_handlers.BlobstoreUploadHandler, PageHandler):
	def get(self):
		if self.user:
			templateVals = {'me': self.user}
			self.render('usersettings.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if self.user:
			name = self.request.get('name')
			alt_email = self.request.get('alt_email')
			password = self.request.get('password')
			verifyPassword = self.request.get('verifyPassword')
			country = self.request.get('country')
			facebook = self.request.get('facebook')
			youtube = self.request.get('youtube')
			google_plus = self.request.get('google_plus')
			twitter = self.request.get('twitter')
			pinterest = self.request.get('pinterest')
			tumblr = self.request.get('tumblr')
			website = self.request.get('website')
			city = self.request.get('city')
			phone_num = self.request.get('phone_num')
			
			user = self.user
			user.city = city
			user.phone_num = phone_num
			user = update_user_name(name, user)
			user = update_user_alt_email(alt_email, user)
			user = update_user_country(country, user)
			user = update_social_profiles(facebook, youtube, google_plus, twitter, pinterest, tumblr, website, user)
			user.put()
			self.redirect('/usersettings')
		else:
			self.redirect('/')
			
class ExploreHandler(PageHandler):               ## TODO
	def post(self):
		self.redirect('/')

class SearchResultsHandler(PageHandler):           ## TODO
	def get(self):
		if self.user:
			templateVals = {'me': self.user}
			self.render('search_results.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if not self.user:
			templateVals = {'me': ""}
		else:
			templateVals = {'me': self.user}
		search_string = self.request.get('search_string')
		photos_qry = Picture.query(ndb.OR(Picture.caption == search_string, Picture.tags == search_string, Picture.albums == search_string)).order(-Picture.viewed)
		blogs_qry = Blog.query(Blog.title == search_string)
		groups_qry = Group.query(Group.name == search_string)
		users_qry = User.query(ndb.OR(User.name == search_string, User.wpc_name == search_string, User.email == search_string, User.photography_interests == search_string))

		photos = photos_qry.fetch(100)
		blogs = blogs_qry.fetch(100)
		groups = groups_qry.fetch(100)
		users = users_qry.fetch(100)

		templateVals['photos'] = photos
		templateVals['blogs'] = blogs
		templateVals['groups'] = groups
		templateVals['users'] = users
		self.render('search_results.html', **templateVals)

class ForumHandler(PageHandler):    ## TODO
	def get(self):
		if not self.user:
			templateVals = {'me': ""}
			self.render('forum.html', **templateVals)
		else:
			templateVals = {'me': self.user}
			self.render('forum.html', **templateVals)

	def post(self):
		if self.user:
			title = self.request.get('title')
			content = self.request.get('content')
			if title and content:
				create_blog(title, content, self.user.key)
				self.redirect('/%s/forum' % self.user.key.id())
			else:
				errorMsg = "Please enter both title and content!"
				templateVals = {'me': self.user, 'title': title, 'content': content, 'submitError': errorMsg}
				self.render('forum.html', **templateVals)
		else:
			self.redirect('/')

class GroupsHandler(PageHandler):
	def get(self):
		if not self.user:
			templateVals = {'me': ""}
			self.render('groups.html', **templateVals)
		else:
			templateVals = {'me': self.user}
			mygroups = [grpKey.get() for grpKey in self.user.groups]
			templateVals['mygroups'] = mygroups
			self.render('groups.html', **templateVals)

class PhotosHandler(PageHandler):
	def get(self):
		if not self.user:
			templateVals = {'me': ""}
		else:
			templateVals = {'me': self.user}
		qry = Picture.query()
		qry1 = qry.order(-Picture.viewed)
		qry2 = qry.order(-Picture.awards)
		qry3 = qry.order(-Picture.likes)
		qry4 = qry1
		qry5 = qry.order(-Picture.tags)
		
		most_viewed = qry1.fetch()
		top_100 = qry2.fetch()
		most_liked = qry3.fetch()
		recommended = qry4.fetch()
		tags = qry5.fetch()

		templateVals['top_100'] = top_100
		templateVals['most_viewed'] = most_viewed
		templateVals['most_liked'] = most_liked
		templateVals['recommended'] = recommended
		self.render('photos.html', **templateVals)

	def post(self):
		if not self.user:
			templateVals = {'me': ""}
		else:
			templateVals = {'me': self.user}
		photo_list = self.request.get('photo_list')
		list_type = self.request.get('list_type')

		if list_type == 'default_list':
			qry = Picture.query()
			if photo_list == 'top_100':
				qry1 = qry.order(-Picture.awards)
				templateVals['list_name'] = 'Top 100 photos on WPC'
			elif photo_list == 'most_viewed':
				qry1 = qry.order(-Picture.viewed)
				templateVals['list_name'] = 'Top 1000 Most Viewed photos on WPC'
			elif photo_list == 'most_liked':
				qry1 = qry.order(-Picture.likes)
				templateVals['list_name'] = 'Top 1000 Most Liked photos on WPC'
			elif photo_list == 'recommended':
				qry1 = qry.order(-Picture.viewed)
				templateVals['list_name'] = 'WPC Recommended'
			photos = qry1.fetch(1000)
			templateVals['photos'] = photos
		elif list_type == 'genre':
			photos_qry = Picture.query(Picture.align_genre == photo_list)
			templateVals['photos'] = photos_qry
		self.render('photo_list.html', **templateVals)

class BlogsHandler(PageHandler):
	def get(self):
		if not self.user:
			templateVals = {'me': ""}
		else:
			templateVals = {'me': self.user}
			templateVals['user'] = self.user
			myblogs = Blog.of_ancestor(self.user.key)
			templateVals['myblogs'] = myblogs
		qry = Blog.query()
		qry1 = qry.order(-Blog.created)
		qry2 = qry.order(Blog.created)

		blogs = qry.fetch()
		newest = qry1.fetch()
		oldest = qry2.fetch()

		templateVals['blogs'] = blogs
		templateVals['newest'] = newest
		templateVals['oldest'] = oldest
		self.render('blogs.html', **templateVals)


class UserIdeabookHandler(PageHandler, blobstore_handlers.BlobstoreUploadHandler):
	def get(self, resource):
		#userid = str(urllib.unquote(resource))
		userid = resource
		user = User.get_by_id(userid)
		if user:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			photos = Picture.of_ancestor(user.key)
			templateVals['photos'] = photos
			blogs = Blog.of_ancestor(user.key)
			templateVals['blogs'] = blogs
			uploadUrl = blobstore.create_upload_url('/resource')
			templateVals['uploadUrl'] = uploadUrl
			self.render('user_ideabook.html', **templateVals)
		else:
			self.redirect('/')

class UserStudioHandler(PageHandler, blobstore_handlers.BlobstoreUploadHandler):
	def get(self, resource):
		#userid = str(urllib.unquote(resource))
		userid = resource
		user = User.get_by_id(userid)
		if user:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			photos = Picture.of_ancestor(user.key)
			templateVals['photos'] = photos
			blogs = Blog.of_ancestor(user.key)
			templateVals['blogs'] = blogs
			uploadUrl = blobstore.create_upload_url('/resource')
			templateVals['uploadUrl'] = uploadUrl
			self.render('user_studio.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		userid = self.request.get('user')
		user = User.get_by_id(userid)
		print userid
		if user and self.user == user:
			form = self.request.get('formType')
			action = self.request.get('actionType')
			print form + " : " + action
			if form == "profile_image":
				if action == "select":
					user.avatar = get_key_urlunsafe(self.request.get('avatarKey'))
				elif action == "upload":
					uploads = self.get_uploads('avatarFile')
					blobInfo = uploads[0]
					photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
					user.avatar = photo.key
				elif action == "remove":
					user.avatar = None
			elif form == "cover_image1":
				if action == "select":
					user.cover1 = get_key_urlunsafe(self.request.get('cover1Key'))
				elif action == "upload":
					uploads = self.get_uploads('cover1')
					blobInfo = uploads[0]
					photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
					user.cover1 = photo.key
				elif action == "remove":
					user.cover1 = None
			elif form == "cover_image2":
				if action == "select":
					user.cover2 = get_key_urlunsafe(self.request.get('cover2key'))
				elif action == "upload":
					uploads = self.get_uploads('cover2')
					blobInfo = uploads[0]
					photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
					user.cover2 = photo.key
				elif action == "remove":
					user.cover2 = None
			elif form == "watermark_image":
				if action == "select":
					user.watermark = get_key_urlunsafe(self.request.get('watermark_image_key'))
				elif action == "upload":
					uploads = self.get_uploads('watermark_image_file')
					blobInfo = uploads[0]
					photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
					user.watermark = photo.key
				elif action == "remove":
					user.cover1 = None
			elif form == "photography_types":
				print "photography_types"
				photoType = self.request.get_all('photoType')
				photoTypeDelete = self.request.get_all('photoTypeDelete')
				user.photography_interests += photoType
				for i in photoTypeDelete:
					user.photography_interests.remove(i)
			elif form == "delete":
				photoKey = get_key_urlunsafe(self.request.get('photoKey'))
				delete_photo(photoKey, self.user.key)
			elif form == "edit":
				photoKey = get_key_urlunsafe(self.request.get('photoKey'))
				self.redirect('/editphoto/%s' % photoKey.urlsafe())
			else:
				self.redirect('/' + userid)
			user.put()
			self.redirect('/' + userid)
		elif user != self.user:
			print "user != self.user"
			form = self.request.get('formType')
			action = self.request.get('actionType')
			if form == "personal_message":
				message = self.request.get('message')
			if form == "photography_awards":
				photoAward = self.request.get_all('photoAward')
				user.awards += photoAward
			if form == "follow":
				user_key = self.request.get('user_key')
				self_user_key = self.request.get('self_user_key')
				user_key = [user.key]
				self_user_key = [self.user.key]
				user.following += self_user_key
				self.user.followers += user_key
			else:
				self.redirect('/' + userid)
			user.put()
			self.user.put()
			self.redirect('/' + userid)
		else:
			print "OH NO!"
			self.redirect('/')

class UserEditHandler(PageHandler, blobstore_handlers.BlobstoreUploadHandler):
	def post(self):
		userid = self.request.get('user')
		user = User.get_by_id(userid)
		print "HERE"
		if user and self.user == user:
			form = self.request.get('formType')
			action = self.request.get('actionType')
			print form + " : " + action
			if form == "profile_image":
				if action == "select":
					user.avatar = get_key_urlunsafe(self.request.get('avatarKey'))
				elif action == "upload":
					uploads = self.get_uplaods('avatarFile')
					blobInfo = uploads[0]
					photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
					user.avatar = images.get_serving_url(photo.blobKey)
				elif action == "remove":
					user.avatar = None
			elif form == "cover_image1":
				if action == "select":
					user.cover1 = self.request.get('cover1')
				elif action == "upload":
					uploads = self.get_uplaods('cover1')
					blobInfo = uploads[0]
					photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
					user.cover1 = photo.key
				elif action == "remove":
					user.cover1 = None
			elif form == "cover_image2":
				if action == "select":
					user.cover2 = self.request.get('cover2')
				elif action == "upload":
					uploads = self.get_uplaods('cover2')
					blobInfo = uploads[0]
					photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
					user.cover2 = photo.key
				elif action == "remove":
					user.cover2 = None
			else:
				self.redirect('/' + userid)
			user.put()
			self.redirect('/' + userid)
		else:
			print "OH NO!"
			self.redirect('/')

class UserAboutHandler(PageHandler):
	def get(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		if user:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			photos = Picture.of_ancestor(user.key)
			templateVals['photos'] = photos
			self.render('user_about.html', **templateVals)
		else:
			self.redirect('/')

class UserAboutEditHandler(PageHandler):
	def get(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		if user:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			photos = Picture.of_ancestor(user.key)
			templateVals['photos'] = photos
			self.render('user_about_edit.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		if self.user == user:
			country = self.request.get('country')
			status = self.request.get('status')
			facebook = self.request.get('facebook')
			youtube = self.request.get('youtube')
			google_plus = self.request.get('google_plus')
			twitter = self.request.get('twitter')
			pinterest = self.request.get('pinterest')
			tumblr = self.request.get('tumblr')
			website = self.request.get('website')
			city = self.request.get('city')
			phone_num = self.request.get('phone_num')

			user.city = city
			user.phone_num = phone_num
			user.status = status
			user = update_user_country(country, user)
			user = update_social_profiles(facebook, youtube, google_plus, twitter, pinterest, tumblr, website, user)
			user.put()
			self.redirect('/%s/about' % self.user.key.id())
		else:
			self.redirect('/')


class UserPhotosHandler(PageHandler):
	def get(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		if user:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			photos = Picture.of_ancestor(user.key)
			templateVals['photos'] = photos
			self.render('user_photos.html', **templateVals)
		else:
			self.redirect('/')
			
	def post(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		templateVals = {'me': self.user}
		templateVals['user'] = user
		if self.user == user:
			action = self.request.get('actionType')
			if action == "delete":
				photoKey = get_key_urlunsafe(self.request.get('photoKey'))
				delete_photo(photoKey, self.user.key)
				self.redirect('/%s/photos' % resource)
			elif action == "edit":
				photoKey = get_key_urlunsafe(self.request.get('photoKey'))
				self.redirect('/editphoto/%s' % photoKey.urlsafe())
		elif self.user != user:
			action = self.request.get('actionType')
			if action == "like":
				photoKey = get_key_urlunsafe(self.request.get('photoKey'))
				photo = photoKey.get()
				photo.likes += 1
				photo.put()
				self.redirect('/%s/photos' % resource)
			if action == "add":
				self.redirect('/%s/photos' % resource)
		else:
			self.redirect('/')

class UserBlogsHandler(PageHandler):
	def get(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		if user:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			blogs = Blog.of_ancestor(user.key)
			templateVals['blogs'] = blogs
			self.render('user_blogs.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		if self.user == user:
			action = self.request.get('actionType')
			blogKey = get_key_urlunsafe(self.request.get('blogKey'))
			if action == "delete":
				delete_blog(blogKey, self.user.key)
				self.redirect('/%s/blogs' % resource)
			elif action == "edit":
				self.redirect('/editblog/%s' % blogKey.urlsafe())
		else:
			self.redirect('/')

class UserGroupsHandler(PageHandler):
	def get(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		if user:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			self.render('user_groups.html', **templateVals)
		else:
			self.redirect('/')

class UserQuestionsHandler(PageHandler):
	def get(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		if user:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			self.render('user_questions.html', **templateVals)
		else:
			self.redirect('/')

class PortfolioPermpageHandler(PageHandler):
	def get(self, resource):
		portfolioKey = get_key_urlunsafe(resource)
		portfolio = portfolioKey.get()
		if portfolio:
			userKey = portfolioKey.parent()
			user = userKey.get()
			templateVals = {'me': self.user}
			templateVals['user'] = user
			templateVals['portfolio'] = portfolio
			self.render('portfolioperm.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):       ## FIXME
		portfolioKey = get_key_urlunsafe(resource)
		portfolio = portfolioKey.get()
		action = self.request.get('actionType')
		userKey = portfolioKey.parent()
		user = userKey.get()
		templateVals = {'me': self.user}
		templateVals['user'] = user
		templateVals['portfolio'] = portfolio
		if action == "delete":
			delete_portfolio(portfolioKey, self.user.key)
			self.redirect('/%s' % self.user.key.id())
		if action == "photography_awards":
			photoAward = self.request.get_all('photoAward')
			user.awards += photoAward
			user.put()
			self.render('portfolioperm.html', **templateVals)


class UserPortfolioHandler(PageHandler):
	def get(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		if user:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			photos = Picture.of_ancestor(self.user.key)
			templateVals['photos'] = photos
			self.render('user_portfolios.html', **templateVals)
		else:
			self.redirect('/')



class PortfolioNewHandler(PageHandler ,blobstore_handlers.BlobstoreUploadHandler):
	def get(self):
		if self.user:
			templateVals = {'me': self.user}
			photos = Picture.of_ancestor(self.user.key)
			templateVals['photos'] = photos
			uploadUrl = blobstore.create_upload_url('/newportfolio')
			templateVals['uploadUrl'] = uploadUrl
			self.render('new_portfolio.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if self.user:
			resultphotoList = []
			cover1 = None
			cover2 = None
			portfolio_title = self.request.get('portfolio_title')
			cover_image_1_select = self.request.get('cover_image_1_select')
			cover_image_2_select = self.request.get('cover_image_2_select')
			cover_image_1_upload = self.get_uploads('cover_image_1_upload')
			cover_image_2_upload = self.get_uploads('cover_image_2_upload')
			photoSelect = self.request.get_all('photoSelect')
			photoSelectDelete = self.request.get_all('photoSelectDelete')
			resultphotoList += photoSelect
			for i in photoSelectDelete:
				resultphotoList.remove(i)
			print resultphotoList
			if cover_image_1_select:
				cover1 = self.request.get('cover_image_1_select')
			if cover_image_2_select:
				cover2 = self.request.get('cover_image_2_select')
			if cover_image_1_upload:
				blobInfo = cover_image_1_upload[0]
				photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
				cover1 = images.get_serving_url(photo.blobKey)
			if cover_image_2_upload:
				blobInfo = cover_image_2_upload[0]
				photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
				cover2 = images.get_serving_url(photo.blobKey)
			if cover1 == None:
				cover1_photo = self.user.cover1.get()
				cover1 = images.get_serving_url(cover1_photo.blobKey)
			if cover2 == None:
				cover2_photo = self.user.cover2.get()
				cover2 = images.get_serving_url(cover2_photo.blobKey)
			if portfolio_title:
				portfolio = create_portfolio(portfolio_title, cover1, cover2, resultphotoList, self.user.key)
				self.redirect('/portfolio/%s' % portfolio.key.urlsafe())
			else:
				errorMsg = "Please enter portfolio's name!"
				templateVals = {'me': self.user, 'name': name, 'submitError': errorMsg}
				photos = Picture.of_ancestor(self.user.key)
				templateVals['photos'] = photos
				self.render('new_portfolio.html', **templateVals)
		else:
			self.redirect('/')

class PhotoNewHandler(PageHandler, blobstore_handlers.BlobstoreUploadHandler):
	def get(self):
		if self.user:
			templateVals = {'me': self.user}
			uploadUrl = blobstore.create_upload_url('/newphoto')
			templateVals['uploadUrl'] = uploadUrl
			self.render('upload_photo.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if self.user:
			uploads = self.get_uploads('files')
			captionList = self.request.get_all('caption')
			descriptionList = self.request.get_all('description')
			locationList = self.request.get_all('location')
			print self.request
			if len(uploads)>0:
				for i in range(len(uploads)):
					blobInfo = uploads[i]
					caption = captionList[i]
					description = descriptionList[i]
					location = locationList[i]
					photo = create_picture(blobInfo.key(), caption, description, location, self.user.key)
				self.redirect('/%s/photos' % self.user.key.id())
			else:
				uploadUrl = blobstore.create_upload_url('/newphoto')
				errorMsg = "Please choose a photo!"
				templateVals = {'me': self.user, 'uploadUrl': uploadUrl, 'submitError': errorMsg}
				self.render('upload_photo.html', **templateVals)
		else:
			self.redirect('/')

class PhotoEditHandler(PageHandler):
	def get(self, resource):
		photoKey = get_key_urlunsafe(resource)
		photo = photoKey.get()
		if self.user and photo and (self.user.key == photoKey.parent()):
			templateVals = {'me': self.user}
			userKey = photoKey.parent()
			user = userKey.get()
			templateVals['user'] = user
			templateVals['photo'] = photo
			self.render('photoedit.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		photoKey = get_key_urlunsafe(resource)
		photo = photoKey.get()
		action = self.request.get('actionType')
		if self.user and photo and (self.user.key == photoKey.parent()):
			if action == "photo_details":
				caption = self.request.get('caption')
				description = self.request.get('description')
				location = self.request.get('location')
				camera = self.request.get('camera')
				lense = self.request.get('lense')
				shutter_speed = self.request.get('shutter_speed')
				aperture = self.request.get('aperture')
				iso = self.request.get('iso')
				tagList = self.request.get_all('tags')
				albumList = self.request.get_all('albums')
				photoGenre = self.request.get_all('photoGenre')
				photoGenreDelete = self.request.get_all('photoGenreDelete')

				photo.caption = caption
				photo.description = description
				photo.location = location
				photo.camera = camera
				photo.lense = lense
				photo.shutter_speed = shutter_speed
				photo.aperture = aperture
				photo.iso = iso
				photo.tags += tagList
				photo.albums += albumList
				photo.align_genre += photoGenre
				for i in photoGenreDelete:
					photo.align_genre.remove(i)
				photo.put()
				self.redirect('/%s/photos' % self.user.key.id())
			elif action == "delete":
				delete_photo(photoKey, self.user.key)
				self.redirect('/%s/photos' % self.user.key.id())
			elif action == "rotate_left":
				photo = images.rotate(photoKey, 90)
				photo.put()



class GroupNewHandler(PageHandler ,blobstore_handlers.BlobstoreUploadHandler):
	def get(self):
		if self.user:
			templateVals = {'me': self.user}
			photos = Picture.of_ancestor(self.user.key)
			templateVals['photos'] = photos
			uploadUrl = blobstore.create_upload_url('/newgroup')
			templateVals['uploadUrl'] = uploadUrl
			self.render('new_group.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if self.user:
			action = self.request.get('actionType')
			name = self.request.get('name')
			description = self.request.get('description')
			if action == "select":
				cover = self.request.get('cover_image')
			elif action == "upload":
				uploads = self.get_uploads('cover_photo')
				blobInfo = uploads[0]
				photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
				cover = images.get_serving_url(photo.blobKey)
			if name:
				group = create_group(name, description, cover, self.user.key)
				self.redirect('/group/%s' % group.key.urlsafe())
			else:
				errorMsg = "Please enter group's name!"
				templateVals = {'me': self.user, 'name': name, 'description': description, 'submitError': errorMsg}
				photos = Picture.of_ancestor(self.user.key)
				templateVals['photos'] = photos
				self.render('new_group.html', **templateVals)
		else:
			self.redirect('/')

class BlogNewHandler(PageHandler, blobstore_handlers.BlobstoreUploadHandler):
	def get(self):
		if self.user:
			templateVals = {'me': self.user}
			photos = Picture.of_ancestor(self.user.key)
			templateVals['photos'] = photos
			uploadUrl = blobstore.create_upload_url('/newblog')
			templateVals['uploadUrl'] = uploadUrl
			self.render('new_blog.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if self.user:
			action = self.request.get('actionType')
			title = self.request.get('title')
			content = self.request.get('content')
			if action == "select":
				cover = self.request.get('cover_image')
			elif action == "upload":
				uploads = self.get_uploads('cover_photo')
				blobInfo = uploads[0]
				photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
				cover = images.get_serving_url(photo.blobKey)
			if title and content:
				blog = create_blog(title, content, cover, self.user.key)
				self.redirect('/blog/%s' % blog.key.urlsafe())
			else:
				errorMsg = "Please enter both title and content!"
				templateVals = {'me': self.user, 'title': title, 'content': content, 'submitError': errorMsg}
				photos = Picture.of_ancestor(self.user.key)
				templateVals['photos'] = photos
				self.render('new_blog.html', **templateVals)
		else:
			self.redirect('/')

class BlogEditHandler(PageHandler):
	def get(self, resource):
		blogKey = get_key_urlunsafe(resource)
		blog = blogKey.get()
		if self.user and blog and (self.user.key == blogKey.parent()):
			templateVals = {'me': self.user}
			templateVals['title'] = blog.title
			templateVals['content'] = blog.content
			self.render('edit_blog.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		blogKey = get_key_urlunsafe(resource)
		blog = blogKey.get()
		if self.user and blog and (self.user.key == blogKey.parent()):
			title = self.request.get('title')
			content = self.request.get('content')
			if title and content:
				blog.title = title
				blog.content = content
				blog.put()
				self.redirect('/%s/blogs' % self.user.key.id())
			else:
				errorMsg = "Please enter both title and content!"
				templateVals = {'me': self.user, 'title': title, 'content': content, 'submitError': errorMsg}
				self.render('edit_blog.html', **templateVals)
		else:
			self.redirect('/')

class GroupPermpageHandler(PageHandler):
	def get(self, resource):
		groupKey = get_key_urlunsafe(resource)
		group = groupKey.get()
		if group:
			templateVals = {'me': self.user}
			templateVals['group'] = group
			self.render('groupperm.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):       ## FIXME
		groupKey = get_key_urlunsafe(resource)
		action = self.request.get('actionType')
		if action == "delete":
			delete_group(groupKey, self.user.key)
			self.redirect('/%s' % self.user.key.id())
		elif action == "edit":
			self.redirect('/editgroup/%s' % resource)

class PhotoPermpageHandler(PageHandler):
	def get(self, resource):
		photoKey = get_key_urlunsafe(resource)
		photo = photoKey.get()
		if photo:
			templateVals = {'me': self.user}
			userKey = photoKey.parent()
			user = userKey.get()
			templateVals['user'] = user
			if self.user:
				if self.user != user:
					photo.viewed += 1
					photo.put()
			else:
				photo.viewed += 1
				photo.put()
			templateVals['photos'] = Picture.of_ancestor(userKey)
			templateVals['photo'] = photo
			self.render('photoperm.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		photoKey = get_key_urlunsafe(resource)
		photo = photoKey.get()
		userKey = photoKey.parent()
		user = userKey.get()
		templateVals = {'me': self.user}
		templateVals['photo'] = photo
		templateVals['user'] = user
		templateVals['photos'] = Picture.of_ancestor(userKey)
		if user and self.user and (self.user == user):
			action = self.request.get('actionType')
			if action == "delete":
				delete_photo(photoKey, self.user.key)
				self.redirect('/%s/photos' % self.user.key.id())
			elif action == "edit":
				self.redirect('/editphoto/%s' % photoKey.urlsafe())
			elif action == "add_comment":
				comment = []
				comment.append(self.request.get('comment'))
				photo.comments = comment
				photo.put()
				self.render('photoperm.html', **templateVals)
			elif action == "add_tag_album":
				tagList = self.request.get_all('tags')
				albumList = self.request.get_all('albums')
				photo.tags += tagList
				photo.albums += albumList
				photo.put()
				self.render('photoperm.html', **templateVals)
			elif action == "add_comment":
				comment = self.request.get('comment')
				user_key = self.request.get('user_key')
				photo.comments += comment
				photo.put()
				self.render('photoperm.html', **templateVals)
		elif user != self.user:
			action = self.request.get('actionType')
			if action == "like":	
				photo.likes += 1
				photo.put()
				self.render('photoperm.html', **templateVals)
			elif action == "photography_awards":
				photoAward = self.request.get_all('photoAward')
				photo.awards += photoAward
				photo.put()
				self.render('photoperm.html', **templateVals)
			elif action == "add_comment":
				comment = self.request.get('comment')
				user_key = self.request.get('user_key')
				photo.comments += comment
				photo.put()
				self.render('photoperm.html', **templateVals)
		else:
			self.redirect('/')

class BlogPermpageHandler(PageHandler):
	def get(self, resource):
		blogKey = get_key_urlunsafe(resource)
		blog = blogKey.get()
		if blog:
			templateVals = {'me': self.user}
			userKey = blogKey.parent()
			user = userKey.get()
			templateVals['user'] = user
			if self.user:
				if self.user != user:
					blog.viewed += 1
					blog.put()
			else:
				blog.viewed += 1
				blog.put()
			templateVals['blog'] = blog
			self.render('blogperm.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		blogKey = get_key_urlunsafe(resource)
		blog = blogKey.get()
		templateVals ={'blog': blog, 'me':self.user}
		if user and self.user and (self.user == user):
			action = self.request.get('actionType')
			if action == "delete":
				delete_blog(blogKey, self.user.key)
				self.redirect('/%s/blogs' % self.user.key.id())
			elif action == "edit":
				self.redirect('/editblog/%s' % resource)
			elif action == "like":	
				blog.likes += 1
				blog.put()
				self.render('blogperm.html', **templateVals)
		else:
			self.redirect('/')

class PhotoServeHandler(blobstore_handlers.BlobstoreDownloadHandler):
	def get(self, resource):
		resource = str(urllib.unquote(resource))
		blobInfo = blobstore.BlobInfo.get(resource)
		self.send_blob(blobInfo)

class SigninHandler(PageHandler):
	def get(self):
		if self.user:
			self.redirect('/')
		else:
			self.render("signin.html")

class SignupHandler(PageHandler):
	def post(self):
	#	try:
			name = self.request.get('name')
			wpc_name = self.request.get('wpc_name')
			email = self.request.get('email')
			password = self.request.get('password')
			verifyPassword = self.request.get('verifyPassword')
			templateVals = {'name': name, 'signupEmail': email}
			if name and wpc_name and email and password and (verifyPassword == password):
				prevUser = User.get_by_id(email)
				wpcUser = User.get_by_id(wpc_name)
				if not prevUser:
					if not wpcUser:
						user = create_user(email, name, wpc_name, password)
						self.login(user)
						self.redirect("/")
					else:
						templateVals['signupError'] = "Account already exists for this wpc user name"
				else:
					templateVals['signupError'] = "Account already exists for this Email ID or wpc user name"
			else:
				templateVals['signupError'] = "Enter all fields!"
			self.render('signin.html', **templateVals)
	#	except:
	#		self.page_error()

class LoginHandler(PageHandler):
	def post(self):
	#	try:
			email = self.request.get('email')
			password = self.request.get('password')
			templateVals = {'signinEmail': email}
			if email and password:
				user = User.get_by_id(email)
				if user:
					if utils.valid_password(email, password, user.passwordHash):
						self.login(user)
						self.redirect("/")
					else:
						templateVals['signinError'] = "Invalid password!"
				else:
					templateVals['signinError'] = "Account doesn't exist for this Email ID!"
			else:
				templateVals['signinError'] = "Enter both Email ID and Password!"
			self.render('signin.html', **templateVals)
	#	except:
	#		self.page_error()

class LogoutHandler(PageHandler):
	def get(self):
		self.logout()
		self.redirect('/')

class DefaultHandler(PageHandler):
	def get(self, resource):
		self.redirect('/')

app = webapp2.WSGIApplication([
			('/', MainHandler),
			('/signin', SigninHandler),
			('/signup', SignupHandler),
			('/login', LoginHandler),
			('/logout', LogoutHandler),
			('/groups' , GroupsHandler),
			('/photos' , PhotosHandler),
			('/blogs' , BlogsHandler),
			('/forum', ForumHandler),
			('/explore', ExploreHandler),
			('/search', SearchResultsHandler),
			('/settings', UserSettingsHandler),
			('/home', UserHomeHandler),
			('/group/([^/]+)', GroupPermpageHandler),
			('/photo/([^/]+)', PhotoPermpageHandler),
			('/blog/([^/]+)', BlogPermpageHandler),
			('/portfolio/([^/]+)', PortfolioPermpageHandler),
			('/editphoto/([^/]+)', PhotoEditHandler),
			('/editblog/([^/]+)', BlogEditHandler),
			('/edituser', UserEditHandler),
			('/newgroup', GroupNewHandler),
			('/newportfolio', PortfolioNewHandler),
			('/newphoto', PhotoNewHandler),
			('/newblog', BlogNewHandler),
			('/servephoto/([^/]+)', PhotoServeHandler),
			('/([^/]+)/about', UserAboutHandler),
			('/([^/]+)/editabout', UserAboutEditHandler),
			('/([^/]+)/photos', UserPhotosHandler),
			('/([^/]+)/blogs', UserBlogsHandler),
			('/([^/]+)/groups', UserGroupsHandler),
			('/([^/]+)/q&a', UserQuestionsHandler),
			('/([^/]+)/portfolios', UserPortfolioHandler),
			('/([^/]+)/ideabook', UserIdeabookHandler),
			('/([^/]+)', UserStudioHandler),
			('/([^.]+)', DefaultHandler)
			], debug=True)