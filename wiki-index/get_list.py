import crawler

peoples = {}
lists = crawler.get_people_list(crawler.get_wiki('Lists of people by occupation'))

for k, v in lists.items():
    if v.startswith('/wiki/List_of'):
        l = crawler.get_people_list(crawler.get_wiki(v[6:]))
        peoples = {**peoples, **l}
        print('\r%d' % len(peoples))
        if len(peoples) > 30000: break

cnt = 0

for k, v in peoples.items():
    if cnt > 8700:
        try:
            wiki_page = crawler.get_wiki(v[6:])
            f = open('data/' + k + '.wiki', 'w')
            f.write(wiki_page)
            f.close()
        except:
            print("ERROR")
    cnt += 1
    print('\r%s %d/%d' % (k, cnt, len(peoples)), end='')
