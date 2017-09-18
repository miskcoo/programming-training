#!/usr/bin/python3
# -*- coding: utf-8 -*-

import urllib.parse
import urllib.request
import re
from bs4 import BeautifulSoup

user_agent = r'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.157 Safari/537.36'

def get_wiki(title):
    headers = { 'User-Agent': user_agent }

    url = 'http://en.wikipedia.org/wiki/' + title.replace(' ', '_')
    request = urllib.request.Request(url, headers=headers)
    response = urllib.request.urlopen(request)
    return response.read().decode('utf-8')

def get_sub_info(item_td):
    if item_td.find('div', class_='plainlist'):
        return [ li.get_text().strip() for li in item_td.find_all('li') ]
    elif item_td.find('a', class_='image'):
        return '!?image:' + item_td.img['src']
    return item_td.get_text().strip()

def get_infobox(html_page):
    html_page = re.sub(r'<sup.*?>.*?</sup>', '', html_page)
    soup = BeautifulSoup(html_page, 'html.parser')

    for hidden in soup.find_all(style='display:none'):
        hidden.decompose()

    infobox = soup.find(class_='infobox')
    if not infobox: return None, None

    def get_info(infobox):
        info = {}
        now_info = {}

        caption = infobox.find('caption', recursive=False)
        if caption:
            pass
            info[caption.get_text()] = now_info
            caption.decompose()

        for item in infobox.find_all('tr'):
            if isinstance(item, str):
                continue

            th = item.find('th', recursive=False)
            td = item.find('td', recursive=False)

            if th and td:
                key = th.get_text()
                now_info[key] = get_sub_info(td)
            elif th:
                now_info = {}
                info[th.get_text()] = now_info
            elif td:
                image = td.find(class_='image')
                if image:
                    now_info['image'] = image.img['src']
                elif td.find('div', class_='NavContent'):
                    pass
#                    title = td.find(class_='NavHead').get_text().replace('[hide]', '')
#                    info[title] = { 'sub_table': get_info(td.table) }
                elif td.find('table'):
                    sub_table = td.find('table')
                    sub_info = get_info(sub_table)
                    now_info['sub_table'] = sub_info
                elif td.find('ul', recursive=False):
                    now_info['sub_list'] = [ li.get_text() for li in td.find_all('li') ]
                else:
                    now_info = {}
                    info[td.get_text()] = now_info

        empty_keys = [ k for k, v in info.items() if not v ]
        for key in empty_keys:
            del info[key]
        return info

    title = soup.title.string.replace(' - Wikipedia', '')
    return title, get_info(infobox)

def get_people_list(html_page):
    soup = BeautifulSoup(html_page, 'html.parser')

    peoples = {}
    for item in soup.find(id='mw-content-text').find_all('li'):
        links = item.find_all('a')
        if links and 'href' in links[0].attrs and links[0]['href'].startswith('/wiki'):
            peoples[links[0].get_text()] = links[0]['href']
    return peoples
