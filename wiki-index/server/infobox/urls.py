from django.conf.urls import url
from django.contrib import admin

from . import views

urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'^person/(?P<person_id>\d+)$', views.person, name='person'),
    url(r'^search$', views.search, name='search'),
]
