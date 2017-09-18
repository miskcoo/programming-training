from .models import Infobox
import json

def merge_list(list1, list2):
    for v in list2:
        if v not in list1:
            list1.append(v)
    return list1

def split_keywords(keywords):
    keys = []
    for k in keywords:
        keys = merge_list(keys, k.split(' '))
    if '' in keys: keys.remove('')
    return keys

def search_by_keywords_meta(keywords, search_str):
    matched = {}
    for index, keyword in enumerate(keywords):
        search_dict = { search_str: keyword }
        for info in Infobox.objects.filter(**search_dict):
            if info.id not in matched:
                matched[info.id] = { 'keywords': [index], 'object': info }
            else:
                matched[info.id]['keywords'].append(index)
    return matched

def search_by_keywords(keywords):
    return search_by_keywords_meta(keywords, 'query_str__contains')

def search_by_name(keywords):
    return search_by_keywords_meta(keywords, 'title__contains')

def search_key(key, info):
    for k, v in info.items():
        if k.lower() == key.lower():
            return v
        elif isinstance(v, dict):
            ret = search_key(key, v)
            if ret is not None:
                return ret
    return None

def search_value(keywords, info):
    def one_of(keywords, string):
        matched = []
        for index, key in enumerate(keywords):
            if key.lower() in string.lower():
                matched.append(index)
        return matched

    if not info: return []

    matched = []
    if isinstance(info, dict):
        for elem in info.values():
            matched = merge_list(matched, search_value(keywords, elem))
    elif isinstance(info, list):
        for elem in info:
            matched = merge_list(matched, search_value(keywords, elem))
    else: matched = one_of(keywords, info)
    return matched

def search_matched_value(keywords, info):
    def one_of(keywords, string):
        for index, key in enumerate(keywords):
            if key.lower() in string.lower():
                return True
        return False

    if not info: return []

    matched_info = {}
    if isinstance(info, dict):
        for key, val in info.items():
            sub_info = search_matched_value(keywords, val)
            if sub_info:
                if isinstance(sub_info, bool):
                    matched_info[key] = val
                else:
                    for k, v in sub_info.items():
                        if k.lower() != 'image':
                            if not isinstance(v, str) or not v.startswith('!?'):
                                matched_info[k] = v
        return matched_info
    elif isinstance(info, list):
        for elem in info:
            if one_of(keywords, elem): return True
        return False
    else: return one_of(keywords, info)

def search_by_field(field, keywords):
    mismatch = []
    matched = search_by_keywords(keywords)
    for person_id, val in matched.items():
        keys, info = val['keywords'], val['object']
        person = json.loads(info.json_str)

        field_info = search_key(field, person)
        matched_value = search_value(keywords, field_info)
        if not field_info or not matched_value:
            mismatch.append(person_id)
        else:
            val['keywords'] = matched_value

    for person_id in mismatch:
        del matched[person_id]
    return matched
