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
import random
import json
import math

from math import exp, expm1
from datamodel import *
from datahandle import *
from google.appengine.ext import blobstore
from google.appengine.ext.webapp import blobstore_handlers
from google.appengine.api import images
from google.appengine.ext import ndb
from google.appengine.ext.db import stats
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
			qry_g = Group.query()
			qry_b = Blog.query()
			qry1 = qry.order(-Picture.score)
			qry3 = qry.order(-Picture.created)
			qry4 = User.query(ndb.AND(User.wpc_score >= 5000 , User.wpc_score <= 10000)).order(-User.wpc_score)
			qry5 = qry4.order(-User.joined)
			qry6 = qry_g.order(-Group.members)
			qry8 = qry6.order(-Group.photos)
			qry7 = qry_b.order(-Blog.score)
			new_users = qry5.fetch(6)
			top_score = qry1.fetch(10)
			new_photos = qry3.fetch(20)
			top_blogs = qry7.fetch(3)
			top_groups = qry8.fetch(3)
			templateVals['new_users'] = new_users
			templateVals['new_photos'] = new_photos
			templateVals['top_score'] = top_score
			templateVals['top_blogs'] = top_blogs
			templateVals['top_groups'] = top_groups
			self.render('index.html', **templateVals)
		else:
			templateVals = {'me': self.user}
			qry = Picture.query()
			qry_g = Group.query()
			qry_b = Blog.query()
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			qry1 = qry.order(-Picture.score)
			qry3 = qry.order(-Picture.created)
			qry4 = User.query(ndb.AND(User.wpc_score >= 5000 , User.wpc_score <= 10000)).order(-User.wpc_score)
			qry5 = qry4.order(-User.joined)
			qry6 = qry_g.order(-Group.members)
			qry8 = qry6.order(-Group.photos)
			qry7 = qry_b.order(-Blog.score)
			new_users = qry5.fetch(6)
			top_score = qry1.fetch(12)
			messages = qry2.fetch(25)
			new_photos = qry3.fetch(20)
			top_blogs = qry7.fetch(4)
			top_groups = qry8.fetch(4)
			templateVals['new_users'] = new_users
			templateVals['new_photos'] = new_photos
			templateVals['top_score'] = top_score
			templateVals['messages'] = messages
			templateVals['top_blogs'] = top_blogs
			templateVals['top_groups'] = top_groups
			self.render('index_user.html', **templateVals)
			
class OptionsPhotosPhotographersHandler(PageHandler):
	def post(self):
		action = self.request.get('actionType')
		if action == 'carousel_like':
			print "########## IN LIKE ####################"
			photoKey = get_key_urlunsafe(self.request.get('photoKey'))
			photo = photoKey.get()
			photo.likes += 1
			photo.like_users += [self.user.key]
			photo.put()
			self.user.wpc_score += 100
			self.user.put()
			userKey = photoKey.parent()
			user = userKey.get()
			message_type = 7
			create_message(message_type, 'Photographer liked your photo', self.user.key, photo.key, None, None, user.key)
			#data = "<a href='#'></i><i class='fa fa-thumbs-o-up' style='color:yellow;'></i></a>"
			data = {'success': 1}
			print data
			self.response.headers['Content-Type'] = 'text/json'
			self.response.out.write(json.dumps(data))
		if action == 'photo_list_like':
			print "########## IN List LIKE ####################"
			photoKey = get_key_urlunsafe(self.request.get('photoKey'))
			photo = photoKey.get()
			photo.likes += 1
			photo.like_users += [self.user.key]
			photo.put()
			self.user.wpc_score += 100
			self.user.put()
			userKey = photoKey.parent()
			user = userKey.get()
			message_type = 7
			create_message(message_type, 'Photographer liked your photo', self.user.key, photo.key, None, None, user.key)
			#data = "<a href='#'></i><i class='fa fa-thumbs-o-up' style='color:yellow;'></i></a>"
			data = {'success': 1}
			print data
			self.response.headers['Content-Type'] = 'text/json'
			self.response.out.write(json.dumps(data))
		if action == 'carousel_add':
			print "########## IN ADD ####################"
			photoKey = get_key_urlunsafe(self.request.get('photoKey'))
			photo = photoKey.get()
			photo.ideabook_additions += 1
			photo.ideabook_additions_users += [self.user.key]
			photo.put()
			self.user.pinned_photos.append(photoKey)
			self.user.put()
			userKey = photoKey.parent()
			user = userKey.get()
			#add_output = "<a></i><i class='fa fa-plus' style='color:yellow;'></i></a>"
			message_type = 12
			create_message(message_type, 'Photographer added your photo to his ideabook', self.user.key, photoKey, None, None, user.key)
			data = {'success': 1}
			print data
			self.response.headers['Content-Type'] = 'text/json'
			self.response.out.write(json.dumps(data))
		if action == 'photo_list_add':
			print "########## IN ADD ####################"
			photoKey = get_key_urlunsafe(self.request.get('photoKey'))
			photo = photoKey.get()
			photo.ideabook_additions += 1
			photo.ideabook_additions_users += [self.user.key]
			photo.put()
			self.user.pinned_photos.append(photoKey)
			self.user.put()
			userKey = photoKey.parent()
			user = userKey.get()
			#add_output = "<a></i><i class='fa fa-plus' style='color:yellow;'></i></a>"
			message_type = 12
			create_message(message_type, 'Photographer added your photo to his ideabook', self.user.key, photoKey, None, None, user.key)
			data = {'success': 1}
			print data
			self.response.headers['Content-Type'] = 'text/json'
			self.response.out.write(json.dumps(data))
		if action == 'follow':
			user = self.request.get('user_key')
			self_user = self.request.get('self_user_key')
			user_key = User.get_by_id(user)
			self_user_key = User.get_by_id(self_user)
			userKey = [user_key.key]
			self_userKey = [self_user_key.key]
			user_key.followers+= self_userKey
			self_user_key.following += userKey
			self.user.wpc_score += 100
			user_key.put()
			self.user.put()
			message_type = 8
			create_message(message_type, 'Photographer is following you', self.user.key, self.user.avatar, None, None, user_key.key)
			data = {'success': 1}
			print data
			self.response.headers['Content-Type'] = 'text/json'
			self.response.out.write(json.dumps(data))
		if action == 'mark_read':
			print "####### in mark read ###############"
			qry2 = Messages.query(Messages.read == 0, ancestor=self.user.key)
			unread_msgs = qry2.fetch()
			for msgs in unread_msgs:
				print msgs
				print msgs.read
				msgs.read = 1
				msgs.put()
				print msgs.read
			data = {'success': 1}
			print data
			self.response.headers['Content-Type'] = 'text/json'
			self.response.out.write(json.dumps(data))
		if action == 'notification_count':
			qry2 = Messages.query(Messages.read == 0, ancestor=self.user.key)
			unread_msgs = qry2.fetch()
			count = qry2.count()
			data = {'success': 1, 'notification_count' : count}
			print data
			self.response.headers['Content-Type'] = 'text/json'
			self.response.out.write(json.dumps(data))
		if action == 'get_unseen_msgs':
			qry2 = Messages.query(Messages.read == 0, ancestor=self.user.key)
			unread_msgs = qry2.fetch()
			count = qry2.count()
			msg_key_srting = ""
			print "#############in unread_msgs############"
			print count
			print unread_msgs
			for msg in unread_msgs:
				key = str(msg.key)
				msg_key_srting = msg_key_srting + "('" + key + "')"
				if (count > 1):
					msg_key_srting = msg_key_srting + ","
			print msg_key_srting
			data = {'success': 1, 'unread_msgs': [msg_key_srting]}
			print data
			self.response.headers['Content-Type'] = 'text/json'
			self.response.out.write(json.dumps(data))


class UserHomeHandler(PageHandler):
	def get(self):
		#userid = str(urllib.unquote(resource))
		if self.user:
			templateVals = {'me': self.user}
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			self.render('user_home.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if self.user:
			templateVals = {'me': self.user}
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			action = self.request.get('actionType')
			if action == 'update_status':
				update_status = self.request.get('update_status')
				self.user.status = update_status
				self.user.put()
				message_type = 11
				for f in self.user.followers:
					create_message(message_type, update_status, self.user.key, self.user.avatar, None, None, f.get().key)
			if action == 'comment':
				comment = self.request.get('comment')
				messageKey = get_key_urlunsafe(self.request.get('messageKey'))
				message = messageKey.get()
				message.comments.append(comment)
				message.put()
			self.render('user_home.html', **templateVals)


class UserSettingsHandler(blobstore_handlers.BlobstoreUploadHandler, PageHandler):
	def get(self):
		if self.user:
			templateVals = {'me': self.user}
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			blogs = Blog.of_ancestor(self.user.key)
			templateVals['blogs'] = blogs
			photos = Picture.of_ancestor(self.user.key)
			templateVals['photos'] = photos
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
		action = self.request.get('actionType')
		if action == "explore":
			num = random.randint(1, 4)
			if num == 1:
				qry = Picture.query()
				count = qry.count()
				row_num = random.randint(0, count-1)
				photo = qry.fetch(1, offset=row_num, keys_only=True)
				print count 
				print row_num
				print photo
				print "######################"
				photo_key = photo[0].urlsafe()
				self.redirect('/photo/%s' % photo_key)
			if num == 2:
				qry = User.query()
				count = qry.count()
				row_num = random.randint(0, count-1)
				user = qry.fetch(1, offset=row_num)
				print count 
				print row_num
				print user
				print "######################"
				user_key = user[0].wpc_name
				self.redirect('/%s' % user_key)
			if num == 3:
				qry = Blog.query()
				count = qry.count()
				row_num = random.randint(0, count-1)
				blog = qry.fetch(1, offset=row_num, keys_only=True)
				print count 
				print row_num
				print blog
				print "######################"
				blog_key = blog[0].urlsafe()
				self.redirect('/blog/%s' % blog_key)
			if num == 4:
				qry = Group.query()
				count = qry.count()
				row_num = random.randint(0, count-1)
				group = qry.fetch(1, offset=row_num, keys_only=True)
				print count 
				print row_num
				print group
				print "######################"
				group_key = group[0].urlsafe()
				self.redirect('/group/%s' % group_key)

class SearchResultsHandler(PageHandler):           ## TODO
	def get(self):
		if self.user:
			templateVals = {'me': self.user}
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			self.render('search_results.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if not self.user:
			templateVals = {'me': ""}
		else:
			templateVals = {'me': self.user}
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
		search_string = self.request.get('search_string')
		search_string_lower = search_string.lower()
		search_string_upper = search_string.upper()
		search_string_title = search_string.title()
		photos_qry = Picture.query(ndb.OR(Picture.align_genre == search_string, Picture.align_genre == search_string_lower, Picture.align_genre == search_string_upper, Picture.align_genre == search_string_title, Picture.caption == search_string, Picture.tags == search_string, Picture.albums == search_string, Picture.caption == search_string_lower, Picture.tags == search_string_lower, Picture.albums == search_string_lower, Picture.caption == search_string_upper, Picture.tags == search_string_upper, Picture.albums == search_string_upper, Picture.caption == search_string_title, Picture.tags == search_string_title, Picture.albums == search_string_title)).order(-Picture.viewed)
		blogs_qry = Blog.query(ndb.OR(Blog.title == search_string , Blog.title == search_string_lower, Blog.title == search_string_upper, Blog.title == search_string_title))
		groups_qry = Group.query(ndb.OR(Group.name == search_string, Group.name == search_string_lower, Group.name == search_string_upper, Group.name == search_string_title))
		users_qry = User.query(ndb.OR(User.name == search_string, User.name == search_string_lower, User.name == search_string_upper, User.name == search_string_title, User.wpc_name == search_string, User.email == search_string, User.photography_interests == search_string, User.wpc_name == search_string_lower, User.email == search_string_lower, User.photography_interests == search_string_lower, User.wpc_name == search_string_upper, User.email == search_string_upper, User.photography_interests == search_string_upper, User.wpc_name == search_string_title, User.email == search_string_title, User.photography_interests == search_string_title))
		photos = photos_qry.fetch(50)
		blogs = blogs_qry.fetch(50)
		groups = groups_qry.fetch(50)
		users = users_qry.fetch(50)

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
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			mygroups = [grpKey.get() for grpKey in self.user.groups]
			templateVals['mygroups'] = mygroups
			self.render('groups.html', **templateVals)

class PhotosHandler(PageHandler):
	def get(self):
		if not self.user:
			templateVals = {'me': ""}
		else:
			templateVals = {'me': self.user}
			qry7 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry7.fetch()
			templateVals['messages'] = messages
		qry = Picture.query()
		qry_p = User.query()
		qry1 = qry.order(-Picture.viewed)
		qry2 = qry.order(-Picture.score)
		qry3 = qry.order(-Picture.likes)
		qry4 = qry1
		qry5 = qry.order(-Picture.tags)
		qry6 = qry_p.order(-User.wpc_score)
		
		most_viewed = qry1.fetch(5)
		top_100 = qry2.fetch(100)
		most_liked = qry3.fetch(5)
		recommended = qry4.fetch(5)
		tags = qry5.fetch()
		top_users = qry6.fetch(6)

		templateVals['top_100'] = top_100
		templateVals['top_users'] = top_users
		templateVals['most_viewed'] = most_viewed
		templateVals['most_liked'] = most_liked
		templateVals['recommended'] = recommended
		self.render('photos.html', **templateVals)

	def post(self):
		if not self.user:
			templateVals = {'me': ""}
		else:
			templateVals = {'me': self.user}
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
		photo_list = self.request.get('photo_list')
		list_type = self.request.get('list_type')

		if list_type == 'default_list':
			qry = Picture.query()
			if photo_list == 'top_100':
				qry1 = qry.order(-Picture.score)
				templateVals['list_name'] = 'Top 100 photos on WPC'
				photos = qry1.fetch(100)
			elif photo_list == 'most_viewed':
				qry1 = qry.order(-Picture.viewed)
				templateVals['list_name'] = 'Top 1000 Most Viewed photos on WPC'
				photos = qry1.fetch(1000)
			elif photo_list == 'most_liked':
				qry1 = qry.order(-Picture.likes)
				templateVals['list_name'] = 'Top 1000 Most Liked photos on WPC'
				photos = qry1.fetch(1000)
			elif photo_list == 'recommended':
				qry1 = qry.order(-Picture.viewed)
				templateVals['list_name'] = 'WPC Recommended'
				photos = qry1.fetch(1000)
			templateVals['photos'] = photos
		elif list_type == 'genre':
			photos_qry = Picture.query(Picture.align_genre == photo_list)
			templateVals['photos'] = photos_qry
		elif list_type == 'search':
			search_string = self.request.get('search')
			photos_qry = Picture.query(ndb.OR(Picture.caption == search_string, Picture.tags == search_string, Picture.albums == search_string)).order(-Picture.viewed)
			templateVals['photos'] = photos_qry
		self.render('photo_list.html', **templateVals)


class PhotoListPageHandler(PageHandler): 
	def post(self):
		print "########## IN Post ####################"
		if not self.user:
			print "########## IN Not User ####################"
			templateVals = {'me': ""}
			qry = Picture.query()
			qry1 = qry.order(-Picture.awards)
			photos = qry1.fetch(10)
			templateVals['photos'] = photos
			templateVals['list_name'] = 'Top 1000 Most Viewed photos on WPC'
			self.render('photo_list.html', **templateVals)	
		else:
			print "########## IN User ####################"
			templateVals = {'me': self.user}
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
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
			qry4 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry4.fetch()
			templateVals['messages'] = messages
		qry = Blog.query()
		qry1 = qry.order(-Blog.created)
		qry2 = qry.order(Blog.created)
		qry3 = qry.order(-Blog.viewed)
		qry4 = qry.order(-Blog.score)
		qry5 = qry4.order(-Blog.created)

		blogs = qry.fetch(100)
		newest = qry1.fetch(100)
		oldest = qry2.fetch(100)
		views = qry3.fetch(5)
		highest_rating = qry4.fetch(100)
		trendiest = qry5.fetch(100)

		templateVals['blogs'] = blogs
		templateVals['newest'] = newest
		templateVals['oldest'] = oldest
		templateVals['views'] = views
		templateVals['highest_rating'] = highest_rating
		templateVals['trendiest'] = trendiest
		self.render('blogs.html', **templateVals)
		

class UserIdeabookHandler(PageHandler, blobstore_handlers.BlobstoreUploadHandler):
	def get(self, resource):
		#userid = str(urllib.unquote(resource))
		userid = resource
		user = User.get_by_id(userid)
		if user:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			self.render('user_ideabook.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		templateVals = {'me': self.user}
		templateVals['user'] = user
		if self.user:
			action = self.request.get('actionType')
			if action == "like":
				photoKey = get_key_urlunsafe(self.request.get('photoKey'))
				photo = photoKey.get()
				photo.likes += 1
				photo.put()
				self.user.wpc_score += 100
				self.user.put()
			if action == "delete":
				photoKey = get_key_urlunsafe(self.request.get('photoKey'))
				user.pinned_photos.remove(photoKey)
				user.put()
			self.render('user_ideabook.html', **templateVals)


class UserStudioHandler(PageHandler, blobstore_handlers.BlobstoreUploadHandler):
	def get(self, resource):
		#userid = str(urllib.unquote(resource))
		userid = resource
		user = User.get_by_id(userid)
		print "user"
		print "###############################################"
		if user:
			print "in if user"
			print "###############################################"
			templateVals = {'me': self.user}
			templateVals['user'] = user
			photos = Picture.of_ancestor(user.key)
			templateVals['photos'] = photos
			blogs = Blog.of_ancestor(user.key)
			templateVals['blogs'] = blogs
			uploadUrl = blobstore.create_upload_url('/resource')
			templateVals['uploadUrl'] = uploadUrl
			if self.user == user:
				qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
				messages = qry2.fetch()
				templateVals['messages'] = messages
			elif self.user and user != self.user:
				user.profile_views += 1
				user.viewed_by += [self.user.key]
				user.put()
			else:
				user.profile_views += 1
				user.put()
			print "2 in if user"
			print "###############################################"
			self.render('user_studio.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		userid = self.request.get('user')
		user = User.get_by_id(userid)
		print "###############################################"
		print userid
		print "###############################################"
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
					photo.is_avatar_pic = 1
					photo.put()
					user.avatar = photo.key
				elif action == "remove":
					user.avatar = None
				user.put()
				self.redirect('/' + userid)	
			elif form == "cover_image1":
				if action == "select":
					print "######################in cover 1 select #########################"
					user.cover1 = get_key_urlunsafe(self.request.get('cover1Key'))
				elif action == "upload":
					uploads = self.get_uploads('cover1')
					blobInfo = uploads[0]
					photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
					user.cover1 = photo.key
				elif action == "remove":
					user.cover1 = None
				user.put()
				self.redirect('/' + userid)	
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
				user.put()
				self.redirect('/' + userid)	
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
				user.put()
				self.redirect('/' + userid)	
			elif form == "photography_types":
				print "photography_types"
				photoType = self.request.get_all('photoType')
				photoTypeDelete = self.request.get_all('photoTypeDelete')
				for i in photoTypeDelete:
					photoType.remove(i)
				user.photography_interests += photoType
				user.put()
				self.redirect('/' + userid)	
			elif form == "photo":
				print "##################### In photo ##########################"
				if action == "delete":
					print "##################### In photo delete ##########################"
					photoKey = get_key_urlunsafe(self.request.get('photoKey'))
					delete_photo(photoKey, self.user.key)
					self.redirect('/' + userid)
				elif action == "edit":
					print "##################### In photo edit ##########################"
					photoKey = get_key_urlunsafe(self.request.get('photoKey'))
					print photoKey.urlsafe()
					self.redirect('/editphoto/%s' % photoKey.urlsafe())
			elif form == "group":
				print "##################### In delete group ##########################"
				if action == "delete_group":
					groupKey = get_key_urlunsafe(self.request.get('groupKey'))
					print "###############################################"
					print groupKey
					delete_group(groupKey, self.user.key)
					self.redirect('/' + userid)
			elif form == "blog":
				print "##################### In delete group ##########################"
				if action == "delete_blog":
					blogKey = get_key_urlunsafe(self.request.get('blogKey'))
					print "###############################################"
					delete_blog(blogKey, self.user.key)
					self.redirect('/' + userid)
			elif form == "portfolio":
				if action == "delete":
					if self.user:
						portfolioKey = self.request.get('portfolioKey')
						portfolio = get_key_urlunsafe(portfolioKey)
						delete_portfolio(portfolio, self.user.key)
					self.redirect('/' + userid)
				if action == "edit":
					portfolioKey = self.request.get('portfolioKey')
					portfolio = get_key_urlunsafe(portfolioKey)
					self.redirect('/editportfolio/%s' % portfolioKey)
			else:
				self.redirect('/' + userid)
		elif user != self.user:
			print "user != self.user"
			form = self.request.get('formType')
			action = self.request.get('actionType')
			if form == "personal_message":
				print "In awards"
				message = self.request.get('message')
				message_type = 4
				if self.user.avatar == None:
					create_message(message_type, message, self.user.key, self.user.avatar, None, None, user.key)
				else:
					create_message(message_type, message, self.user.key, None, None, None, user.key)
			if form == "photography_awards":
				photoAward = self.request.get_all('photoAward')
				user.awards += photoAward
				user.awarded_by += [self.user.key]
				self.user.wpc_score += 100
				message_type = 5
				for a in photoAward:
					if self.user.avatar:
						create_message(message_type, a, self.user.key, self.user.avatar, None, None, user.key)
					else:
						create_message(message_type, a, self.user.key, None, None, None, user.key)
				print self.user.avatar
			if form == "follow":
				user_key = self.request.get('user_key')
				self_user_key = self.request.get('self_user_key')
				user_key = [user.key]
				self_user_key = [self.user.key]
				user.followers += self_user_key
				self.user.following += user_key
				self.user.wpc_score += 100
				message_type = 8
				create_message(message_type, 'Photographer is following you', self.user.key, self.user.avatar, None, None, user.key)
			if form == "photo":
				if action == "like":
					photoKey = get_key_urlunsafe(self.request.get('photoKey'))
					photo = photoKey.get()
					photo.likes += 1
					photo.put()
					self.user.wpc_score += 100
					self.user.put()
					userKey = photoKey.parent()
					user = userKey.get()
					message_type = 7
					create_message(message_type, 'Photographer liked your photo', self.user.key, photo.key, None, None, user.key)
				elif action == "add":
					photoKey = get_key_urlunsafe(self.request.get('photoKey'))
					self.user.pinned_photos.append(photoKey)
					self.user.put()
					message_type = 12
					create_message(message_type, 'Photographer added your photo to his ideabook', self.user.key, photoKey, None, None, user.key)
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
			if self.user:
				photos = Picture.of_ancestor(self.user.key)
				templateVals['photos'] = photos
				qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
				messages = qry2.fetch()
				templateVals['messages'] = messages
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
			if self.user:
				photos = Picture.of_ancestor(self.user.key)
				templateVals['photos'] = photos
				qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
				messages = qry2.fetch()
				templateVals['messages'] = messages
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
			if self.user:
				qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
				messages = qry2.fetch()
				templateVals['messages'] = messages
			self.render('user_photos.html', **templateVals)
		else:
			self.redirect('/')
			
	def post(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		templateVals = {'me': self.user}
		templateVals['user'] = user
		qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
		messages = qry2.fetch()
		templateVals['messages'] = messages
		if self.user == user:
			action = self.request.get('actionType')
			if action == "delete":
				photoKey = get_key_urlunsafe(self.request.get('photoKey'))
				delete_photo(photoKey, self.user.key)
				self.redirect('/%s/photos' % resource)
			elif action == "edit":
				photoKey = get_key_urlunsafe(self.request.get('photoKey'))
				self.redirect('/editphoto/%s' % photoKey.urlsafe())
			elif action == "filter_tag":
				tag_name = self.request.get('tag_name')
				photos = Picture.query(Picture.tags == tag_name, ancestor=self.user.key)
				templateVals['photos'] = photos
				self.render('user_photos.html', **templateVals)
			elif action == "filter_album":
				album_name = self.request.get('album_name')
				photos = Picture.query(Picture.albums == album_name, ancestor=self.user.key)
				templateVals['photos'] = photos
				self.render('user_photos.html', **templateVals)
			elif action == "search":
				print "in search"
				search_string = self.request.get('search_string')
				photos = Picture.query(Picture.caption == search_string, ancestor=self.user.key)
				templateVals['photos'] = photos
				self.render('user_photos.html', **templateVals)
		elif self.user != user:
			action = self.request.get('actionType')
			if action == "filter_tag":
				tag_name = self.request.get('tag_name')
				photos = Picture.query(Picture.tags == tag_name, ancestor=user.key)
				templateVals['photos'] = photos
				self.render('user_photos.html', **templateVals)
			elif action == "filter_album":
				album_name = self.request.get('album_name')
				photos = Picture.query(Picture.albums == album_name, ancestor=user.key)
				templateVals['photos'] = photos
				self.render('user_photos.html', **templateVals)
			elif action == "search":
				print "in search"
				search_string = self.request.get('search_string')
				print search_string
				photos = Picture.query(Picture.caption == search_string, ancestor=user.key)
				templateVals['photos'] = photos
				self.render('user_photos.html', **templateVals)
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
			if self.user:
				qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
				messages = qry2.fetch()
				templateVals['messages'] = messages
			self.render('user_blogs.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		templateVals = {'me': self.user}
		templateVals['user'] = user
		blogs = Blog.of_ancestor(user.key)
		templateVals['blogs'] = blogs
		if self.user:
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
		if self.user == user:
			action = self.request.get('actionType')
			blogKey = get_key_urlunsafe(self.request.get('blogKey'))
			if action == "delete_blog":
				delete_blog(blogKey, self.user.key)
				self.render('user_blogs.html', **templateVals)
		else:
			self.redirect('/')

class UserGroupsHandler(PageHandler):
	def get(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		if user:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			self.render('user_groups.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		userid = resource
		user = User.get_by_id(userid)
		templateVals = {'me': self.user}
		templateVals['user'] = user
		qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
		messages = qry2.fetch()
		templateVals['messages'] = messages
		if self.user == user:
			print "inside if user"
			action = self.request.get('actionType')
			groupKey = get_key_urlunsafe(self.request.get('groupKey'))
			if action == "delete_group":
				print "inside delete"
				print "####################################################"
				delete_group(groupKey, self.user.key)
				print "####################################################"
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
			if self.user:
				photos = Picture.of_ancestor(self.user.key)
				templateVals['photos'] = photos
				qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
				messages = qry2.fetch()
				templateVals['messages'] = messages
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
		if self.user:
			photos = Picture.of_ancestor(self.user.key)
			templateVals['photos'] = photos
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
		templateVals['portfolio'] = portfolio
		if action == "delete":
			if self.user:
				delete_portfolio(portfolioKey, self.user.key)
				self.render('user_portfolios.html', **templateVals)
		if action == "photography_awards":
			photoAward = self.request.get_all('photoAward')
			user.awards += photoAward
			user.put()
			self.user.wpc_score += 100
			self.user.put()
			message_type = 5
			for a in photoAward:
				if self.user.avatar:
					create_message(message_type, a, self.user.key, self.user.avatar, None, None, user.key)
				else:
					create_message(message_type, a, self.user.key, None, None, None, user.key)
			self.render('portfolioperm.html', **templateVals)
		if action == "select1":
			cover1 = self.request.get('cover1Key')
			portfolio.cover_photo1 = cover1
			portfolio.put()
			self.render('portfolioperm.html', **templateVals)
		if action == "select2":
			cover2 = self.request.get('cover2Key')
			portfolio.cover_photo2 = cover2
			portfolio.put()
			self.render('portfolioperm.html', **templateVals)
		if action == "edit":
			photoKey = get_key_urlunsafe(self.request.get('photoKey'))
			self.redirect('/editphoto/%s' % photoKey.urlsafe())
		if action == "like":
			photoKey = get_key_urlunsafe(self.request.get('photoKey'))
			photo = photoKey.get()
			photo.likes += 1
			photo.put()
			userKey = photoKey.parent()
			user = userKey.get()
			resultphotoList = []
			blog = create_blog("", "", "", self.user.key)
			group = create_group("", "", "", resultphotoList, self.user.key)
			message_type = 7
			create_message(message_type, 'Photographer liked your photo', self.user.key, photo.key, blog.key, group.key, user.key)
			self.user.wpc_score += 100
			self.user.put()
			self.render('portfolioperm.html', **templateVals)
		if action == "add":
			photoKey = get_key_urlunsafe(self.request.get('photoKey'))
			self.user.pinned_photos.append(photoKey)
			self.user.put()
			self.render('portfolioperm.html', **templateVals)
		if action == "portfolio_edit":
			portfolioKey = self.request.get('portfolioKey')
			portfolio = get_key_urlunsafe(portfolioKey)
			self.redirect('/editportfolio/%s' % portfolioKey)
		if action == "delete_photo":
			photoKey = get_key_urlunsafe(self.request.get('photoKey'))
			portfolio.photos.remove(photoKey)
			portfolio.put()
			self.render('portfolioperm.html', **templateVals)
		if action == "add_photos":
			resultphotoList = []
			temp1 = []
			temp2 = []
			photoSelect = self.request.get_all('photoSelect')
			photoSelectDelete = self.request.get_all('photoSelectDelete')
			for w in photoSelect:
				x = get_key_urlunsafe(w)
				temp1.append(x)
			for r in photoSelectDelete:
				x = get_key_urlunsafe(r)
				temp2.append(x)
			resultphotoList += temp1
			for i in temp2:
				resultphotoList.remove(i)
			portfolio.photos += resultphotoList
			portfolio.put()
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
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			self.render('user_portfolios.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):       ## FIXME
		userid = resource
		user = User.get_by_id(userid)
		templateVals = {'me': self.user}
		templateVals['user'] = user
		photos = Picture.of_ancestor(self.user.key)
		templateVals['photos'] = photos
		portfolioKey = self.request.get('portfolioKey')
		portfolio = get_key_urlunsafe(portfolioKey)
		action = self.request.get('actionType')
		qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
		messages = qry2.fetch()
		templateVals['messages'] = messages
		print portfolio.parent()
		print portfolioKey
		if action == "delete":
			if self.user:
				delete_portfolio(portfolio, self.user.key)
				self.render('user_portfolios.html', **templateVals)
		if action == "edit":
			self.render('user_portfolios.html', **templateVals)



class PortfolioNewHandler(PageHandler ,blobstore_handlers.BlobstoreUploadHandler):
	def get(self):
		if self.user:
			templateVals = {'me': self.user}
			photos = Picture.of_ancestor(self.user.key)
			templateVals['photos'] = photos
			uploadUrl = blobstore.create_upload_url('/newportfolio')
			templateVals['uploadUrl'] = uploadUrl
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			self.render('new_portfolio.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if self.user:
			resultphotoList = []
			temp1 = []
			temp2 = []
			cover1 = ""
			cover2 = ""
			portfolio_title = self.request.get('portfolio_title')
			cover_image_1_select = self.request.get('cover_image_1_select')
			cover_image_2_select = self.request.get('cover_image_2_select')
			cover_image_1_upload = self.get_uploads('cover_image_1_upload')
			cover_image_2_upload = self.get_uploads('cover_image_2_upload')
			photoSelect = self.request.get_all('photoSelect')
			photoSelectDelete = self.request.get_all('photoSelectDelete')
			for w in photoSelect:
				x = get_key_urlunsafe(w)
				temp1.append(x)
			for r in photoSelectDelete:
				x = get_key_urlunsafe(r)
				temp2.append(x)
			resultphotoList += temp1
			for i in temp2:
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
			if cover1 == "":
				cover1 = self.user.cover1_url
			if cover2 == "":
				cover2 = self.user.cover2_url
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

class PhotoUploadStatusHandler(PageHandler, blobstore_handlers.BlobstoreUploadHandler):
	def get(self):
		if self.user:
			data = {'progress': self.user.progress_bar_val}
			print data
			self.response.headers['Content-Type'] = 'text/json'
			self.response.out.write(json.dumps(data))
		else:
			self.redirect('/')

class PhotoNewHandler(PageHandler, blobstore_handlers.BlobstoreUploadHandler):
	def get(self):
		if self.user:
			templateVals = {'me': self.user}
			uploadUrl = blobstore.create_upload_url('/newphoto')
			templateVals['uploadUrl'] = uploadUrl
			photos = Picture.of_ancestor(self.user.key)
			templateVals['photos'] = photos
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			self.render('upload_photo.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if self.user:
			action = self.request.get('actionType')
			if action == "file_upload":
				uploads = self.get_uploads('files')
				captionList = self.request.get_all('caption')
				descriptionList = self.request.get_all('description')
				locationList = self.request.get_all('location')
				#add_location = self.request.get('add_location')
				tag = self.request.get('add_tag')
				album = self.request.get('add_album')
				print tag
				print album
				print "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
				num_files = len(uploads) 
				increment = 100/num_files
				progress = 0
				if num_files > 0:
					for i in range(len(uploads)):
						blobInfo = uploads[i]
						caption = captionList[i]
						description = descriptionList[i]
						location = locationList[i]
						photo = create_multiple_picture(blobInfo.key(), caption, description, location, album, tag, self.user.key)
						progress = progress + increment
						self.user.progress_bar_val = progress
						self.user.put()
					message_type = 1
					for f in self.user.followers:
						create_message(message_type, 'Photographer added new photos', self.user.key, photo.key, None, None, f.get().key)
					self.user.progress_bar_val = 0
					self.user.put()
					self.redirect('/%s/photos' % self.user.key.id())
				else:
					uploadUrl = blobstore.create_upload_url('/newphoto')
					errorMsg = "Please choose a photo!"
					templateVals = {'me': self.user, 'uploadUrl': uploadUrl, 'submitError': errorMsg}
					self.render('upload_photo.html', **templateVals)
			if action=="track_progress":
				initial_photo_count = self.request.get('initial_photo_count')
				print "in track_progress"
				print count
				if count > initial_photo_count:
					increment = count - initial_photo_count
					data = {'progress': increment}
				else:
					data = {'progress': 0}
				print data
				print "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
				self.response.headers['Content-Type'] = 'text/json'
				self.response.out.write(json.dumps(data))
		else:
			self.redirect('/')

class PortfolioEditHandler(PageHandler ,blobstore_handlers.BlobstoreUploadHandler):
	def get(self, resource):
		portfolioKey = get_key_urlunsafe(resource)
		portfolio = portfolioKey.get()
		print "####### in portfolio edit ############"
		print portfolio
		if self.user and portfolio and (self.user.key == portfolioKey.parent()):
			templateVals = {'me': self.user}
			photos = Picture.of_ancestor(self.user.key)
			templateVals['photos'] = photos
			uploadUrl = blobstore.create_upload_url('/editportfolio/resource')
			templateVals['uploadUrl'] = uploadUrl
			templateVals['portfolio'] = portfolio
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			self.render('portfolio_edit.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		print "####### in portfolio edit post ############"
		print resource
		portfolioKey = self.request.get('portfolioKey')
		portfolio = get_key_urlunsafe(portfolioKey)
		#portfolioKey = get_key_urlunsafe(resource)
		portfolio = portfolio.get()
		print portfolio
		if self.user:
			resultphotoList = []
			temp1 = []
			temp2 = []
			cover1 = None
			cover2 = None
			portfolio_title = self.request.get('portfolio_title')
			cover_image_1_select = self.request.get('cover_image_1_select')
			cover_image_2_select = self.request.get('cover_image_2_select')
			cover_image_1_upload = self.get_uploads('cover_image_1_upload')
			cover_image_2_upload = self.get_uploads('cover_image_2_upload')
			photoSelect = self.request.get_all('photoSelect')
			photoSelectDelete = self.request.get_all('photoSelectDelete')
			if photoSelect or photoSelectDelete:
				for w in photoSelect:
					x = get_key_urlunsafe(w)
					temp1.append(x)
				for r in photoSelectDelete:
					x = get_key_urlunsafe(r)
					temp2.append(x)
				resultphotoList += temp1
				for i in temp2:
					portfolio.photos.remove(i)
				portfolio.photos += resultphotoList
			if cover_image_1_select:
				cover1 = self.request.get('cover_image_1_select')
				portfolio.cover_photo1 = cover1
			if cover_image_2_select:
				cover2 = self.request.get('cover_image_2_select')
				portfolio.cover_photo2 = cover2
			if cover_image_1_upload:
				blobInfo = cover_image_1_upload[0]
				photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
				cover1 = images.get_serving_url(photo.blobKey)
				portfolio.cover_photo1 = cover1
			if cover_image_2_upload:
				blobInfo = cover_image_2_upload[0]
				photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
				cover2 = images.get_serving_url(photo.blobKey)
				portfolio.cover_photo2 = cover2
			if portfolio_title:
				portfolio.name = portfolio_title
			print portfolio.name
			portfolio.put()
			self.redirect('/portfolio/%s' % portfolio.key.urlsafe())
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
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
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
				img = images.Image(str(photo.blobKey))
				img.resize(width=1920, height=1080)
				thumbnail = img.execute_transforms(output_encoding=images.JPEG)
				photo.put()
				self.redirect('/%s/photos' % self.user.key.id())



class GroupNewHandler(PageHandler ,blobstore_handlers.BlobstoreUploadHandler):
	def get(self):
		if self.user:
			templateVals = {'me': self.user}
			photos = Picture.of_ancestor(self.user.key)
			templateVals['photos'] = photos
			uploadUrl = blobstore.create_upload_url('/newgroup')
			templateVals['uploadUrl'] = uploadUrl
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			self.render('new_group.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if self.user:
			action = self.request.get('actionType')
			name = self.request.get('name')
			description = self.request.get('description')
			temp1 = []
			temp2 = []
			resultphotoList = []
			cover = ""
			if action == "select":
				cover = self.request.get('cover_image')
			elif action == "upload":
				uploads = self.get_uploads('cover_photo')
				blobInfo = uploads[0]
				photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
				cover = images.get_serving_url(photo.blobKey)
			photoSelect = self.request.get_all('photoSelect')
			photoSelectDelete = self.request.get_all('photoSelectDelete')
			for w in photoSelect:
				x = get_key_urlunsafe(w)
				temp1.append(x)
			for r in photoSelectDelete:
				x = get_key_urlunsafe(r)
				temp2.append(x)
			resultphotoList += temp1
			if cover == "":
				cover = self.user.cover1_url
			if name:
				group = create_group(name, description, cover, resultphotoList, self.user.key)
				message_type = 3
				for f in self.user.followers:
					create_message(message_type, 'Photographer added new Group', self.user.key, self.user.avatar, None, group.key, f.get().key)
				self.redirect('/group/%s' % group.key.urlsafe())
			else:
				errorMsg = "Please enter group's name!"
				templateVals = {'me': self.user, 'name': name, 'description': description, 'submitError': errorMsg}
				photos = Picture.of_ancestor(self.user.key)
				templateVals['photos'] = photos
				self.render('new_group.html', **templateVals)
		else:
			self.redirect('/')

class GroupEditHandler(PageHandler ,blobstore_handlers.BlobstoreUploadHandler):
	def get(self, resource):
		groupKey = get_key_urlunsafe(resource)
		group = groupKey.get()
		userKey = groupKey.parent()
		user = userKey.get()
		templateVals = {'me': self.user}
		uploadUrl = blobstore.create_upload_url('/editgroup/resource')
		templateVals['uploadUrl'] = uploadUrl
		if group:
			templateVals['user'] = user
			templateVals['group'] = group
			if self.user:
				templateVals['photos'] = Picture.of_ancestor(self.user.key)
				qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
				messages = qry2.fetch()
				templateVals['messages'] = messages
			self.render('edit_group.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		groupKey = get_key_urlunsafe(self.request.get('groupKey'))
		group = groupKey.get()
		userKey = groupKey.parent()
		user = userKey.get()
		if self.user:
			action = self.request.get('actionType')
			name = self.request.get('name')
			description = self.request.get('description')
			temp1 = []
			temp2 = []
			resultphotoList = []
			cover = ""
			if action == "select":
				print "########### in select ##############"
				cover = self.request.get('cover_image')
			elif action == "upload":
				print "########### in upload ##############"
				uploads = self.get_uploads('cover_photo')
				blobInfo = uploads[0]
				photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
				cover = images.get_serving_url(photo.blobKey)
			photoSelect = self.request.get_all('photoSelect')
			photoSelectDelete = self.request.get_all('photoSelectDelete')
			if photoSelect:
				for w in photoSelect:
					x = get_key_urlunsafe(w)
					temp1.append(x)
			group.photos += temp1
			if photoSelectDelete:
				for r in photoSelectDelete:
					x = get_key_urlunsafe(r)
					group.photos.remove(x)
			if name:
				group.name = name
			if description:
				group.description = description
			print cover
			if cover:
				print "########### in cover ############"
				group.cover_photo = cover
			group.put()
			self.redirect('/group/%s' % group.key.urlsafe())
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
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			self.render('new_blog.html', **templateVals)
		else:
			self.redirect('/')

	def post(self):
		if self.user:
			action = self.request.get('actionType')
			title = self.request.get('title')
			content = self.request.get('content')
			cover = ""
			if action == "select":
				cover = self.request.get('cover_image')
			elif action == "upload":
				uploads = self.get_uploads('cover_photo')
				blobInfo = uploads[0]
				photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
				cover = images.get_serving_url(photo.blobKey)
			if cover == "":
				cover = self.user.cover1_url
			if title and content:
				blog = create_blog(title, content, cover, self.user.key)
				message_type = 2
				for f in self.user.followers:
					create_message(message_type, 'Photographer added new Blog', self.user.key, self.user.avatar, blog.key, None, f.get().key)
				self.redirect('/blog/%s' % blog.key.urlsafe())
			else:
				errorMsg = "Please enter both title and content!"
				templateVals = {'me': self.user, 'title': title, 'content': content, 'submitError': errorMsg}
				photos = Picture.of_ancestor(self.user.key)
				templateVals['photos'] = photos
				self.render('new_blog.html', **templateVals)
		else:
			self.redirect('/')

class BlogEditHandler(PageHandler, blobstore_handlers.BlobstoreUploadHandler):
	def get(self, resource):
		blogKey = get_key_urlunsafe(resource)
		blog = blogKey.get()
		if self.user and blog and (self.user.key == blogKey.parent()):
			templateVals = {'me': self.user}
			uploadUrl = blobstore.create_upload_url('/editblog/resource')
			templateVals['uploadUrl'] = uploadUrl
			photos = Picture.of_ancestor(self.user.key)
			templateVals['photos'] = photos
			templateVals['title'] = blog.title
			templateVals['content'] = blog.content
			templateVals['cover'] = blog.cover
			templateVals['blog'] = blog
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
			self.render('edit_blog.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):
		blogKey = get_key_urlunsafe(self.request.get('blogKey'))
		blog = blogKey.get()
		if self.user and blog and (self.user.key == blogKey.parent()):
			title = self.request.get('title')
			content = self.request.get('content')
			action = self.request.get('actionType')
			cover = ""
			if action == "select":
				cover = self.request.get('cover_image')
			elif action == "upload":
				uploads = self.get_uploads('cover_photo')
				blobInfo = uploads[0]
				photo = create_picture(blobInfo.key(), None, None, None, self.user.key)
				cover = images.get_serving_url(photo.blobKey)
			if title:
				blog.title = title
			if content:
				blog.content = content
			if cover:
				blog.cover = cover
			blog.put()
			self.redirect('/blog/%s' % blog.key.urlsafe())
		else:
			self.redirect('/')

class GroupPermpageHandler(PageHandler):
	def get(self, resource):
		groupKey = get_key_urlunsafe(resource)
		group = groupKey.get()
		userKey = groupKey.parent()
		user = userKey.get()
		if group:
			templateVals = {'me': self.user}
			templateVals['user'] = user
			templateVals['group'] = group
			if self.user:
				templateVals['photos'] = Picture.of_ancestor(self.user.key)
				qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
				messages = qry2.fetch()
				templateVals['messages'] = messages
			self.render('groupperm.html', **templateVals)
		else:
			self.redirect('/')

	def post(self, resource):       ## FIXME
		groupKey = get_key_urlunsafe(resource)
		group = groupKey.get()
		userKey = groupKey.parent()
		user = userKey.get()
		templateVals = {'me': self.user}
		templateVals['user'] = user
		templateVals['group'] = group
		if self.user:
			templateVals['photos'] = Picture.of_ancestor(self.user.key)
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
		temp1 = []
		temp2 = []
		resultphotoList = []
		action = self.request.get('actionType')
		if action == "delete":
			delete_group(groupKey, self.user.key)
			self.redirect('/%s' % self.user.key.id())
		elif action == "edit":
			self.redirect('/editgroup/%s' % resource)
		elif action == "add_photos":
			photoSelect = self.request.get_all('photoSelect')
			photoSelectDelete = self.request.get_all('photoSelectDelete')
			for w in photoSelect:
				x = get_key_urlunsafe(w)
				temp1.append(x)
			for r in photoSelectDelete:
				x = get_key_urlunsafe(r)
				temp2.append(x)
			resultphotoList += temp1
			for i in temp2:
				resultphotoList.remove(i)
			group.photos += resultphotoList
			group.put()
		elif action == "join_group":
			usr_key = self.request.get('self_user_key')
			self_user_key = [self.user.key]
			group.members += self_user_key
			self.user.groups.append(groupKey)
			message_type = 9
			create_message(message_type, 'Photographer joined your group', self.user.key, self.user.avatar, None, group.key, user.key)
			self.user.put()	
			group.put()
		elif action == "delete_photo":
			photoKey = get_key_urlunsafe(self.request.get('photoKey'))
			group.photos.remove(photoKey)
			group.put()
		elif action == "add_comment":
			content = self.request.get('comment')
			comment = create_comment(content, self.user.key)
			comment_list = [comment.key]
			group.comments += comment_list
			group.put()
			self.user.wpc_score += 100
			self.user.put()
			message_type = 15
			create_message(message_type, content, self.user.key, None, None, group.key, user.key)
		self.render('groupperm.html', **templateVals)

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
					photo.viewed_by += [self.user.key]
					photo.put()
			else:
				photo.viewed += 1
				photo.put()
			templateVals['photos'] = Picture.of_ancestor(userKey)
			templateVals['photo'] = photo
			if self.user:
				qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
				messages = qry2.fetch()
				templateVals['messages'] = messages
			if photo.likes > 0:
				likes = math.log(photo.likes,10)
			else:
				likes = 0
			if len(photo.comments) > 0:
				print "in comments"
				print len(photo.comments)
				comments = math.log(len(photo.comments),10)
				print comments
			else:
				comments = 0
			if len(photo.awards) > 0:
				awards = math.log(len(photo.awards),10)
			else:
				awards = 0
			if photo.shares > 0:
				shares = math.log(photo.shares,10)
			else:
				shares = 0
			if photo.viewed > 0:
				views = math.log(photo.viewed,10)
			else:
				views = 0
			if photo.ideabook_additions > 0:
				ideabook_additions = math.log(photo.ideabook_additions,10)
			else:
				ideabook_additions = 0
			photo.score = likes + comments + awards + shares + views + ideabook_additions
			photo.score = round(photo.score, 3)
			photo.put()
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
		if self.user:
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
		templateVals['photos'] = Picture.of_ancestor(userKey)
		if user and self.user and (self.user == user):
			action = self.request.get('actionType')
			if action == "delete":
				delete_photo(photoKey, self.user.key)
				self.redirect('/%s/photos' % self.user.key.id())
			elif action == "edit":
				self.redirect('/editphoto/%s' % photoKey.urlsafe())
			elif action == "add_tag_album":
				tagList = self.request.get_all('tags')
				albumList = self.request.get_all('albums')
				photo.tags += tagList
				photo.albums += albumList
				photo.put()
				self.render('photoperm.html', **templateVals)
			elif action == "add_comment":
				content = self.request.get('comment')
				user_key = self.request.get('user_key')
				comment = create_comment(content, self.user.key)
				comment_list = [comment.key]
				photo.comments += comment_list
				if photo.likes > 0:
					likes = math.log(photo.likes,10)
				else:
					likes = 0
				if len(photo.comments) > 0:
					print "in comments"
					print len(photo.comments)
					comments = math.log(len(photo.comments),10)
					print comments
				else:
					comments = 0
				if len(photo.awards) > 0:
					awards = math.log(len(photo.awards),10)
				else:
					awards = 0
				if photo.shares > 0:
					shares = math.log(photo.shares,10)
				else:
					shares = 0
				if photo.viewed > 0:
					views = math.log(photo.viewed,10)
				else:
					views = 0
				if photo.ideabook_additions > 0:
					ideabook_additions = math.log(photo.ideabook_additions,10)
				else:
					ideabook_additions = 0
				photo.score = likes + comments + awards + shares + views + ideabook_additions
				photo.score = round(photo.score, 3)
				photo.put()
				self.user.wpc_score += 100
				self.user.put()
				self.render('photoperm.html', **templateVals)
		elif user != self.user:
			action = self.request.get('actionType')
			if action == "photography_awards":
				photoAward = self.request.get_all('photoAward')
				photo.awards += photoAward
				if photo.likes > 0:
					likes = math.log(photo.likes,10)
				else:
					likes = 0
				if len(photo.comments) > 0:
					print "in comments"
					print len(photo.comments)
					comments = math.log(len(photo.comments),10)
					print comments
				else:
					comments = 0
				if len(photo.awards) > 0:
					awards = math.log(len(photo.awards),10)
				else:
					awards = 0
				if photo.shares > 0:
					shares = math.log(photo.shares,10)
				else:
					shares = 0
				if photo.viewed > 0:
					views = math.log(photo.viewed,10)
				else:
					views = 0
				if photo.ideabook_additions > 0:
					ideabook_additions = math.log(photo.ideabook_additions,10)
				else:
					ideabook_additions = 0
				photo.score = likes + comments + awards + shares + views + ideabook_additions
				photo.score = round(photo.score, 3)
				photo.put()
				self.user.wpc_score += 100
				self.user.put()
				message_type = 6
				for a in photoAward:
					create_message(message_type, a, self.user.key, photo.key, None, None, user.key)
				self.render('photoperm.html', **templateVals)
			elif action == "add_comment":
				content = self.request.get('comment')
				user_key = self.request.get('user_key')
				comment = create_comment(content, self.user.key)
				comment_list = [comment.key]
				photo.comments += comment_list
				if photo.likes > 0:
					likes = math.log(photo.likes,10)
				else:
					likes = 0
				if len(photo.comments) > 0:
					print "in comments"
					print len(photo.comments)
					comments = math.log(len(photo.comments),10)
					print comments
				else:
					comments = 0
				if len(photo.awards) > 0:
					awards = math.log(len(photo.awards),10)
				else:
					awards = 0
				if photo.shares > 0:
					shares = math.log(photo.shares,10)
				else:
					shares = 0
				if photo.viewed > 0:
					views = math.log(photo.viewed,10)
				else:
					views = 0
				if photo.ideabook_additions > 0:
					ideabook_additions = math.log(photo.ideabook_additions,10)
				else:
					ideabook_additions = 0
				photo.score = likes + comments + awards + shares + views + ideabook_additions
				photo.score = round(photo.score, 3)
				photo.put()
				self.user.wpc_score += 100
				self.user.put()
				message_type = 13
				create_message(message_type, content, self.user.key, photo.key, None, None, user.key)
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
				qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
				messages = qry2.fetch()
				templateVals['messages'] = messages
				if self.user != user:
					blog.viewed += 1
					blog.viewed_by += [self.user.key]
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
		userKey = blogKey.parent()
		user = userKey.get()
		templateVals ={'blog': blog, 'me':self.user}
		templateVals['user'] = user
		if self.user:
			qry2 = Messages.query(ancestor=self.user.key).order(-Messages.created)
			messages = qry2.fetch()
			templateVals['messages'] = messages
		if self.user == user:
			action = self.request.get('actionType')
			if action == "delete":
				delete_blog(blogKey, self.user.key)
				self.redirect('/%s/blogs' % self.user.key.id())
			elif action == "edit":
				self.redirect('/editblog/%s' % blogKey.urlsafe())
			elif action == "add_comment":
				content = self.request.get('comment')
				comment = create_comment(content, self.user.key)
				comment_list = [comment.key]
				blog.comments += comment_list
				if blog.likes > 0:
					likes = math.log(blog.likes,10)
				else:
					likes = 0
				if len(blog.comments) > 0:
					print "in comments"
					print len(blog.comments)
					comments = math.log(len(blog.comments),10)
					print comments
				else:
					comments = 0
				if blog.shares > 0:
					shares = math.log(blog.shares,10)
				else:
					shares = 0
				if blog.viewed > 0:
					views = math.log(blog.viewed,10)
				else:
					views = 0
				if len(blog.ideabook_additions) > 0:
					ideabook_additions = math.log(len(blog.ideabook_additions),10)
				else:
					ideabook_additions = 0
				blog.score = likes + comments + shares + views + ideabook_additions
				blog.score = round(blog.score, 3)
				blog.put()
				self.user.wpc_score += 100
				self.user.put()
				self.render('blogperm.html', **templateVals)
		else:
			action = self.request.get('actionType')
			if action == "like":	
				blog.likes += 1
				if blog.likes > 0:
					likes = math.log(blog.likes,10)
				else:
					likes = 0
				if len(blog.comments) > 0:
					print "in comments"
					print len(blog.comments)
					comments = math.log(len(blog.comments),10)
					print comments
				else:
					comments = 0
				if blog.shares > 0:
					shares = math.log(blog.shares,10)
				else:
					shares = 0
				if blog.viewed > 0:
					views = math.log(blog.viewed,10)
				else:
					views = 0
				if len(blog.ideabook_additions) > 0:
					ideabook_additions = math.log(len(blog.ideabook_additions),10)
				else:
					ideabook_additions = 0
				blog.score = likes + comments + shares + views + ideabook_additions
				blog.score = round(blog.score, 3)
				blog.put()
				message_type = 10
				create_message(message_type, 'Photographer liked your blog', self.user.key, self.user.avatar, blog.key, None, user.key)
				self.render('blogperm.html', **templateVals)
			if action == "add":
				blogKey = get_key_urlunsafe(self.request.get('blogKey'))
				self.user.pinned_blogs.append(blogKey)
				blog.ideabook_additions += [self.user.key]
				if blog.likes > 0:
					likes = math.log(blog.likes,10)
				else:
					likes = 0
				if len(blog.comments) > 0:
					print "in comments"
					print len(blog.comments)
					comments = math.log(len(blog.comments),10)
					print comments
				else:
					comments = 0
				if blog.shares > 0:
					shares = math.log(blog.shares,10)
				else:
					shares = 0
				if blog.viewed > 0:
					views = math.log(blog.viewed,10)
				else:
					views = 0
				if len(blog.ideabook_additions) > 0:
					ideabook_additions = math.log(len(blog.ideabook_additions),10)
				else:
					ideabook_additions = 0
				blog.score = likes + comments + shares + views + ideabook_additions
				blog.score = round(blog.score, 3)
				blog.put()
				self.user.put()
				self.render('blogperm.html', **templateVals)
			if action == "add_comment":
				content = self.request.get('comment')
				comment = create_comment(content, self.user.key)
				comment_list = [comment.key]
				blog.comments += comment_list
				if blog.likes > 0:
					likes = math.log(blog.likes,10)
				else:
					likes = 0
				if len(blog.comments) > 0:
					print "in comments"
					print len(blog.comments)
					comments = math.log(len(blog.comments),10)
					print comments
				else:
					comments = 0
				if blog.shares > 0:
					shares = math.log(blog.shares,10)
				else:
					shares = 0
				if blog.viewed > 0:
					views = math.log(blog.viewed,10)
				else:
					views = 0
				if len(blog.ideabook_additions) > 0:
					ideabook_additions = math.log(len(blog.ideabook_additions),10)
				else:
					ideabook_additions = 0
				blog.score = likes + comments + shares + views + ideabook_additions
				blog.score = round(blog.score, 3)
				blog.put()
				self.user.wpc_score += 100
				self.user.put()
				message_type = 14
				create_message(message_type, content, self.user.key, None, blog.key, None, user.key)
				self.render('blogperm.html', **templateVals)


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
					templateVals['signupError'] = "Account already exists for this Email ID"
			else:
				templateVals['signupError'] = "Enter all fields!"
			self.render('signin.html', **templateVals)
	#	except:
	#		self.page_error()

class LoginHandler(PageHandler):
	def post(self):
	#	try:
			#email = self.request.get('email')
			password = self.request.get('password')
			wpc_name = self.request.get('wpc_name')
			templateVals = {'signinEmail': wpc_name}
			print wpc_name
			print password
			if wpc_name and password:
				print "in if 1"
				print wpc_name
				user = User.get_by_id(wpc_name)
				print user
				if user:
					print "in if 2"
					if utils.valid_password(wpc_name, password, user.passwordHash):
						print "in if 3"
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
			('/options', OptionsPhotosPhotographersHandler),
			('/signin', SigninHandler),
			('/signup', SignupHandler),
			('/login', LoginHandler),
			('/logout', LogoutHandler),
			('/groups' , GroupsHandler),
			('/photos' , PhotosHandler),
			('/photo_lists' , PhotoListPageHandler),
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
			('/editportfolio/([^/]+)', PortfolioEditHandler),
			('/editblog/([^/]+)', BlogEditHandler),
			('/editgroup/([^/]+)', GroupEditHandler),
			('/edituser', UserEditHandler),
			('/newgroup', GroupNewHandler),
			('/newportfolio', PortfolioNewHandler),
			('/newphoto', PhotoNewHandler),
			('/uploadstatus', PhotoUploadStatusHandler),
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