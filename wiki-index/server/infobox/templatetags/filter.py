from django import template

register = template.Library()

@register.filter
def get_type(value):
    return type(value).__name__

@register.filter
def strip(value):
    if isinstance(value, str):
        return value.strip().replace('\n', '<br/>')
    return value

@register.filter
def is_image(s):
    return s.startswith('!?image:')

@register.filter
def cut_image(s):
    return s.replace('!?image:', '')
