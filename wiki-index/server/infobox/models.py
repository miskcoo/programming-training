#!/usr/bin/python3
# -*- coding: utf8 -*-

from django.db import models

class Infobox(models.Model):
    query_str = models.TextField()
    json_str = models.TextField()
    title = models.TextField()
