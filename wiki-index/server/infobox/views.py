from django.http import HttpResponse
from django.shortcuts import get_object_or_404, render
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from .models import Infobox
import json
from .utils import *

default_fields = [ 'born', 'died', 'nationality', 'known for' ]

def render_search_result(info, fields=None, keywords=None):
    person_id = info.id
    person_name = info.title
    person = json.loads(info.json_str)
    sub_person = {}
    if fields:
        for field in fields:
            val = search_key(field, person)
            if val: sub_person[field] = val
    if keywords:
        sub_key = search_matched_value(keywords, person)
        for k, v in sub_key.items():
            if k not in sub_person:
                sub_person[k] = v

    return { 'person_id': int(person_id),
             'person_name': person_name,
             'person': { 'not_display': sub_person }
           }

def person(request, person_id):
    person_info = get_object_or_404(Infobox, pk=person_id)
    person = json.loads(person_info.json_str)
    return render(request, 'person.html',
                  { 'person_id': int(person_id),
                    'person': person,
                    'person_name': person_info.title,
                    'highlight': request.GET.getlist('highlight') })

def index(request):
    return render(request, 'index.html')

def search(request):
    keywords = split_keywords(request.GET.getlist('q'))
    if keywords:
        matched = search_by_keywords(keywords)
    else: matched = None

    fields = request.GET.getlist('f')
    for key in request.GET:
        if not key.startswith('f_'):
            continue

        field = key[2:].replace('_', ' ')
        f_keys = split_keywords(request.GET.getlist(key))
        if not f_keys: continue

        if field not in fields:
            fields.append(field)

        if field != 'name':
            matched_f = search_by_field(field, f_keys)
        else: matched_f = search_by_name(f_keys)

        mismatch = []
        if matched is not None:
            for key, val in matched.items():
                if key not in matched_f:
                    mismatch.append(key)
                else: val['keywords'].extend(matched_f[key]['keywords'])
            for key in mismatch:
                del matched[key]
        else: matched = matched_f

        keywords = merge_list(keywords, f_keys)

    matched = [ (val['keywords'], val['object']) for val in matched.values() ]
    matched.sort(key=lambda x: len(x[0]), reverse=True)

    if fields:
        fields = merge_list(fields, default_fields)
        results = [ render_search_result(obj, fields=fields) for _, obj in matched ]
    else:
        results = [ render_search_result(obj, fields=default_fields, keywords=keywords) for _, obj in matched ]

    page = int(request.GET['page']) if 'page' in request.GET else 1
    page_size = int(request.GET['page_size']) if 'page_size' in request.GET else 10

    paginator = Paginator(results, page_size)
    try:
        contents = paginator.page(page)
    except PageNotAnInteger:
        # If page is not an integer, deliver first page.
        contents = paginator.page(1)
    except EmptyPage:
        # If page is out of range (e.g. 9999), deliver last page of results.
        contents = paginator.page(paginator.num_pages)

    show_page_range_min = contents.number - 5
    show_page_range_max = contents.number + 5
    if show_page_range_max > paginator.num_pages:
        show_page_range_max = paginator.num_pages
        show_page_range_min = show_page_range_max - 10
    elif show_page_range_min <= 1:
        show_page_range_min = 1
        show_page_range_max = show_page_range_min + 10

    show_page_range_max = min(show_page_range_max, paginator.num_pages)
    show_page_range_min = max(show_page_range_min, 1)
    show_page_range = range(show_page_range_min, show_page_range_max + 1)

    return render(request, 'search.html',
                  { 'keywords': keywords,
                    'results': contents,
                    'show_page_range': show_page_range })
