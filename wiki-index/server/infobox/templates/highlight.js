function highlight_keywords(keywords)
{
	var pattern = new RegExp('(' + keywords.join('|') + ')', 'gi');
	var infos = $('.info');
	for(var i = 0; i < infos.length; ++i)
	{
		var src_str = $(infos[i]).html();
		src_str = src_str.replace(pattern, '<mark>$1</mark>')
						 .replace(/(<mark>[^<>]*)((<[^>]+>)+)([^<>]*<\/mark>)/, '$1</mark>$2<mark>$4');
		$(infos[i]).html(src_str);
	}
}

$('document').ready( function() {
	highlight_keywords(search_keywords);
} );
