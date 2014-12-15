from datamodel import *
from google.appengine.ext import blobstore
import utils
import logging

def create_user(email, name, wpc_name, password):
	user = User(id=wpc_name, name=name, wpc_name=wpc_name, email=email, score=0)
	user.passwordHash = utils.hash_password(wpc_name, password)
	userKey = user.put()
	return user

def update_user_name(name, user):
	if name:
		user.name = name
	return user

def update_user_alt_email(email, user):
	if email:
		user.alt_email = email
	return user

def update_user_country(country, user):
	if country:
		user.country = country
	return user

def update_user_profile_photo(photo_key, user):
	if photo_key:
		user.avatar = "/photo/" + photo_key
	return user

def update_social_profiles(facebook, youtube, google_plus, twitter, pinterest, tumblr, website, user):
	if facebook:
		user.facebook=facebook
	if youtube:
		user.youtube=youtube
	if google_plus:
		user.google_plus=google_plus
	if twitter:	
		user.twitter=twitter
	if pinterest:
		user.pinterest=pinterest
	if tumblr:
		user.tumblr=tumblr
	if website:
		user.website=website
	return user

def create_blog(title, content, cover_photo, parent_key):
	blog = Blog(title=title, content=content, cover=cover_photo, parent=parent_key)
	blogKey = blog.put()
	return blog

def create_picture(blobKey, caption, description, location, parent_key):
	pic = Picture(blobKey=blobKey, caption=caption, description=description, location=location, parent=parent_key)
	picKey = pic.put()
	return pic

def create_multiple_picture(blobKey, caption, description, location, album, tag, parent_key):
	pic = Picture(blobKey=blobKey, caption=caption, description=description, location=location, parent=parent_key)
	if tag:
		print tag
		pic.tags += [tag]
	if album:
		print album
		pic.albums += [album]
	print "########################"
	picKey = pic.put()
	return pic

def create_group(name, description, cover_photo, resultphotoList, parent_key):
	grp = Group(name=name, description=description, cover_photo=cover_photo, photos=resultphotoList, parent=parent_key)
	grp.members.append(parent_key)
	grpKey = grp.put()
	user = parent_key.get()
	user.groups.append(grpKey)
	userKey = user.put()
	return grp

def create_portfolio(name, cover1, cover2, resultphotoList, parent_key):
	portfolio = Portfolio(name=name, cover_photo1=cover1, cover_photo2=cover2, photos=resultphotoList, parent=parent_key)
	portfolioKey = portfolio.put()
	user = parent_key.get()
	user.portfolios.append(portfolioKey)
	userKey = user.put()
	return portfolio

def create_message(message_type, message, sender, message_photo, message_blog, message_group, parent_key):
	message = Messages(message_type=message_type, message=message, sender=sender, message_photo=message_photo, message_blog=message_blog, message_group=message_group, parent=parent_key)
	messageKey = message.put()
	user = parent_key.get()
	userKey = user.put()

def create_comment(content, parent_key):
	comment = Comment(content=content, parent=parent_key)
	commentKey = comment.put()
	return comment
	
def add_member(grpKey, userKey):
	grp = grpKey.get()
	user = userKey.get()
	grp.members.append(userKey)
	user.groups.append(grpKey)
	grpKey = grp.put()
	userKey = user.put()

def delete_group(group_key, user_key):
	if user_key == group_key.parent():
		user = user_key.get()
		user.groups.remove(group_key)
		userKey = user.put()
		group_key.delete()
	return

def delete_portfolio(portfolio_key, user_key):
	if user_key == portfolio_key.parent():
		user = user_key.get()
		user.portfolios.remove(portfolio_key)
		userKey = user.put()
		portfolio_key.delete()
	return

def delete_blog(blog_key, user_key):
	if user_key == blog_key.parent():
		blog_key.delete()
	return

def delete_photo(photo_key, user_key):
	if user_key == photo_key.parent():
		blobKey = photo_key.get().blobKey
		blobInfo = blobstore.BlobInfo.get(blobKey)
		blobInfo.delete()
		photo_key.delete()
	return

def get_key_urlunsafe(urlsafeKey):
	try:
		unsafeKey = ndb.Key(urlsafe=urlsafeKey)
	except:
		unsafeKey = None
	return unsafeKey

def get_edit_photo_urlstring(photoList):
	urlString = ""
	for photo in photoList:
		urlString = urlString + str(photo.key.id()) + '+'
	urlString = urlString[:-1]
	return urlString

def get_photo_urlstring(photo):
	urlString = ""
	urlString = urlString + str(photo.key.id())
	urlString = urlString[:-1]
	return urlString

def get_photolist_from_urlstring(urlString, userKey):
	photoList = []
	if urlString:
		photoIdList = urlString.split(" ")
		for photoId in photoIdList:
			photo = ndb.Key('Picture', int(photoId), parent=userKey).get()
			photoList.append(photo)
			logging.info(photoId)
	else:
		logging.error("Empty urlString!")
	return photoList