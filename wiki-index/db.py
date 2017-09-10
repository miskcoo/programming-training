#!/use/bin/python3
# -*- coding: utf8 -*-
import sqlite3
import crawler
import json
import fileinput

conn = sqlite3.connect('server/db.sqlite3')

def get_query_info(infobox):
    if isinstance(infobox, str):
        if infobox.startswith('!?image'):
            return ""
        return infobox
    elif isinstance(infobox, list):
        return ' '.join(infobox)

    s = ""
    for k, v in infobox.items():
        if k == 'image':
            continue
        s += " " + get_query_info(v)
    return s


def add_info(c, filename):
    data = open(filename, 'r').read()
    title, infobox = crawler.get_infobox(data)
    if not infobox: return

    print(infobox)

    s = c.execute('SELECT id FROM `infobox_infobox` WHERE title=?', [title])
    if s.fetchone() is None:
        c.execute('INSERT INTO `infobox_infobox` (title, json_str, query_str) VALUES (?, ?, ?)',
                  [title, json.dumps(infobox), title + " " + get_query_info(infobox)])
    else: print("ERROR")

filenames = [ f for f in fileinput.input() ]

c = conn.cursor()
cnt = 0
for f in filenames:
    try:
        add_info(c, f.strip())
        print('\r%d/%d, %s' % (cnt, len(filenames), f), end='')
    except:
        pass
    cnt += 1
    if cnt % 100 == 0:
        c.execute('COMMIT')
#f = input()
#add_info(f)
