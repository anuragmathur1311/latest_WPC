from google.appengine.ext import ndb
from google.appengine.api import images

class User(ndb.Model):
	name = ndb.StringProperty(required=True)
	wpc_name = ndb.StringProperty(required=True)
	email = ndb.StringProperty(required=True)
	passwordHash = ndb.StringProperty(required=True, indexed=False)
	city = ndb.StringProperty()
	country = ndb.StringProperty()
	alt_email = ndb.StringProperty()
	following = ndb.KeyProperty(kind='User', repeated=True)
	followers = ndb.KeyProperty(kind='User', repeated=True)
	pinned_photos = ndb.KeyProperty(kind='Picture', repeated=True)
	pinned_blogs = ndb.KeyProperty(kind='Blog', repeated=True)
	pinned_groups = ndb.KeyProperty(kind='Group', repeated=True)
	pinned_portfolios = ndb.KeyProperty(kind='Portfolio', repeated=True)
	groups = ndb.KeyProperty(kind='Group', repeated=True)
	portfolios = ndb.KeyProperty(kind='Portfolio', repeated=True)
	joined = ndb.DateTimeProperty(auto_now_add=True)
	status = ndb.StringProperty()
	website = ndb.StringProperty()
	facebook = ndb.StringProperty()
	youtube = ndb.StringProperty()
	google_plus = ndb.StringProperty()
	pinterest = ndb.StringProperty()
	tumblr = ndb.StringProperty()
	twitter = ndb.StringProperty()
	watermark = ndb.KeyProperty(kind='Picture')
	avatar = ndb.KeyProperty(kind='Picture')
	cover1 = ndb.KeyProperty(kind='Picture')
	cover2 = ndb.KeyProperty(kind='Picture')
	score = ndb.IntegerProperty(default=0)
	wpc_score = ndb.IntegerProperty(default=50)
	phone_num = ndb.StringProperty()
	awards = ndb.StringProperty(repeated=True)
	awarded_by = ndb.KeyProperty(kind='User', repeated=True)
	profile_shares = ndb.IntegerProperty(default=0)
	profile_views = ndb.IntegerProperty(default=0)
	viewed_by = ndb.KeyProperty(kind='User', repeated=True)
	favourites = ndb.KeyProperty(repeated=True)
	interests = ndb.KeyProperty(kind='Tag', repeated=True)
	storybook = ndb.KeyProperty(kind='Story', repeated=True)
	notifications = ndb.KeyProperty(kind='Story', repeated=True)
	photography_interests = ndb.StringProperty(repeated=True)
	total_likes = ndb.IntegerProperty(default=0)
	progress_bar_val = ndb.IntegerProperty(default=0)
	
	@property
	def avatar_url(self):
		if self.avatar is None:
			return "/images/default_avatar.png"
		else:
			avatar_photo = self.avatar.get()
			return images.get_serving_url(avatar_photo.blobKey)

	@property
	def cover1_url(self):
		if self.cover1 is None:
			return "/images/default_user_cover2.jpg"
		else:
			cover1_photo = self.cover1.get()
			return images.get_serving_url(cover1_photo.blobKey)

	@property
	def cover2_url(self):
		if self.cover2 is None:
			return "/images/default_user_cover2.jpg"
		else:
			cover2_photo = self.cover2.get()
			return images.get_serving_url(cover2_photo.blobKey)

	@property
	def watermark_url(self):
		if self.watermark is None:
			return "/images/default_avatar.png"
		else:
			watermark_photo = self.watermark.get()
			return images.get_serving_url(watermark_photo.blobKey)

class Group(ndb.Model): # Parent=User (Admin)
	name = ndb.StringProperty(required=True)
	members = ndb.KeyProperty(kind='User', repeated=True)
	photos = ndb.KeyProperty(kind='Picture', repeated=True)
	created = ndb.DateTimeProperty(auto_now_add=True)
	description = ndb.TextProperty()
	cover_photo = ndb.StringProperty()
	comments = ndb.KeyProperty(kind='Comment', repeated=True)

class Portfolio(ndb.Model): # Parent=User (Admin)
	name = ndb.StringProperty(required=True)
	photos = ndb.KeyProperty(kind='Picture', repeated=True)
	created = ndb.DateTimeProperty(auto_now_add=True)
	cover_photo1 = ndb.StringProperty()
	cover_photo2 = ndb.StringProperty()

class Ideabook(ndb.Model):
	name = ndb.StringProperty(required=True)
	created = ndb.DateTimeProperty(auto_now_add=True)
	updated = ndb.DateTimeProperty(auto_now=True)
	photographers = ndb.KeyProperty(kind='User', repeated=True)
	photos = ndb.KeyProperty(kind='Picture', repeated=True)
	blogs = ndb.KeyProperty(kind='Blog', repeated=True)
	groups = ndb.KeyProperty(kind='Group', repeated=True)
	questions = ndb.KeyProperty(kind='Questions', repeated=True)

class Itembook(ndb.Model):
	name = ndb.StringProperty(required=True)
	created = ndb.DateTimeProperty(auto_now_add=True)
	updated = ndb.DateTimeProperty(auto_now=True)

	@classmethod
	def of_ancestor(cls, ancestor_key):
		return cls.query(ancestor=ancestor_key).order(-cls.updated)

class Album(Itembook): # Parent=User
	entries = ndb.KeyProperty(kind='Picture', repeated=True)

class Diary(Itembook): # Parent=User
	entries = ndb.KeyProperty(kind='Blog', repeated=True)

class Post(ndb.Model):
	created = ndb.DateTimeProperty(auto_now_add=True)
	updated = ndb.DateTimeProperty(auto_now=True)
	upvoted = ndb.KeyProperty(kind='User', repeated=True)
	downvoted = ndb.KeyProperty(kind='User', repeated=True)
	likes = ndb.IntegerProperty(default=0)
	viewed = ndb.IntegerProperty(default=0)

	@classmethod
	def of_ancestor(cls, ancestor_key):
		return cls.query(ancestor=ancestor_key).order(-cls.updated)

class Item(Post):
	comments = ndb.KeyProperty(kind='Comment', repeated=True)
	permission = ndb.StringProperty(default='Public')
	visibleTo = ndb.KeyProperty(kind='User', repeated=True)
	followed = ndb.KeyProperty(kind='User', repeated=True)

class Picture(Item): # Parent=User
	caption = ndb.StringProperty()
	location = ndb.StringProperty()
	blobKey = ndb.BlobKeyProperty()
	description = ndb.TextProperty()
	camera = ndb.StringProperty()
	lense = ndb.StringProperty()
	shutter_speed = ndb.StringProperty()
	aperture = ndb.StringProperty()
	iso = ndb.StringProperty()
	tags = ndb.StringProperty(repeated=True)
	albums = ndb.StringProperty(repeated=True)
	awards = ndb.StringProperty(repeated=True)
	align_genre = ndb.StringProperty(repeated=True)
	score = ndb.FloatProperty(default=0)
	ideabook_additions = ndb.IntegerProperty(default=0)
	ideabook_additions_users = ndb.KeyProperty(kind='User', repeated=True)
	like_users = ndb.KeyProperty(kind='User', repeated=True)
	shares = ndb.IntegerProperty(default=0)
	viewed_by = ndb.KeyProperty(kind='User', repeated=True)
	is_avatar_pic = ndb.IntegerProperty(default=0)

class Messages(Item):
	message_type = ndb.IntegerProperty()
	message = ndb.TextProperty()
	sender = ndb.KeyProperty(kind='User')
	message_photo = ndb.KeyProperty(kind='Picture')
	message_blog = ndb.KeyProperty(kind='Blog')
	message_group = ndb.KeyProperty(kind='Group')
	comments = ndb.TextProperty(repeated=True)
	read = ndb.IntegerProperty(default=0)

class Blog(Item): # Parent=User
	title = ndb.StringProperty(required=True)
	content = ndb.TextProperty(required=True)
	cover = ndb.StringProperty()
	score = ndb.FloatProperty(default=0)
	ideabook_additions = ndb.KeyProperty(kind='User', repeated=True)
	shares = ndb.IntegerProperty(default=0)
	viewed_by = ndb.KeyProperty(kind='User', repeated=True)

	@property
	def cover_url(self):
		if self.cover is None:
			return "/images/default_blog_cover.jpg"
		else:
			return self.cover

class Question(Post): # Parent=User
	content = ndb.StringProperty(required=True)
	answers = ndb.KeyProperty(kind='Answer', repeated=True)
	followed = ndb.KeyProperty(kind='User', repeated=True)

class Answer(Post): # Parent=default_user_cover2
	content = ndb.TextProperty(required=True)
	comments = ndb.KeyProperty(kind='Comment', repeated=True)

class Comment(Post): # Parent=User
	content = ndb.TextProperty(required=True)

class Story(ndb.Model): # Parent=User
	caption = ndb.StringProperty(required=True)
	created = ndb.DateTimeProperty(auto_now=True)
	itemKey = ndb.KeyProperty(required=True)

class Tag(ndb.Model):
	name = ndb.StringProperty(required=True)
	items = ndb.KeyProperty(repeated=True)

