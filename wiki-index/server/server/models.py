#!/usr/bin/python3
# -*- coding: utf8 -*-

from django.db import models

class Infobox(models.Model):
    query_str = models.CharField()
    json_str = models.CharField()
    title = models.CharField()
