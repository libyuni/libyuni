
SEO = {
	//! All terms
	terms: {},
	termNames: {},
	//! All articles
	articles: {},
	searchFromObjValue: function(root, index, id) {
		var o = document.getElementById(id);
		if (o)
			this.search(root, index, o.value);
	},
	search: function(root, index, query) {
		var div = document.getElementById('spotlight');
		if (!div)
			return;
		if (!query || query == "") {
			div.style.display = 'none';
			return;
		}
		var tks = query.split(new RegExp("[ ,;/|]", "g"));
		var tokens = new Array();
		var weight = 1.0;
		var registerToken = function(t, i, bonus) {
			var termID = SEO.termNames[t];
			if (termID <= 0)
				return;
			var r = {
				id: termID,
				term: t,
				w: 1.0 * (weight * bonus),
				pages: SEO.terms[termID]
			};
			// updating weights
			// console.log("register  " + t + ",    w:" + r.w);
			if (r.pages) {
				for (var j = 0; j != r.pages.length; ++j) {
					r.pages[j].wResult = r.pages[j].w * r.w;
				}
			}
			tokens.push(r);
		}
		for (var i = 0; i < tks.length; ++i)
		{
			if (tks[i] == "") // invalid token
				continue;
			var t = tks[i];

			if (t == "+")
				weight = 1.0;
			else if (t == "-")
				weight = -1.0;
			else {
				var cont = true;
				do {
					switch (t.charAt(0))
					{
						case '-': weight = 0.01;t = t.substring(1);break;
						case '+': weight = +1.0;t = t.substring(1);break;
						default:cont = false;
					}
				} while (cont);

				var snex = SEO.soundex(t);
				for (var nm in SEO.termNames) {
					if (nm == t) {
						registerToken(nm, i, 1.1);
						continue;
					}
					var sub = (t.length > 2) && (nm.toLowerCase().indexOf(t) != -1);
					if (sub)
						registerToken(nm, i, 0.8);
					else {
						if (SEO.soundex(nm) == snex)
							registerToken(nm, i, 0.7);
					}
				}
			}
		}

		var pages = {};
		for (var i = 0; i != tokens.length; ++i) {
			var r = tokens[i];
			for (var j = 0; j != r.pages.length; ++j) {
				var articleid = r.pages[j].a;
				if (!pages[articleid]) {
					pages[articleid] = {
						a: articleid,
						w: r.pages[j].wResult,
					};
				} else {
					pages[articleid].w *= r.pages[j].wResult;
				}
			}
		}
		var result = [];
		for (var i in pages) {
			var p = pages[i];
			if (p.w > 0.05)
				result.push({id: p.a, w: p.w});
		}
		div.style.display = 'block';
		if (result.length) {
			result.sort(function(a,b) {
				if (a.w < b.w)
					return 1;
				if (a.w > b.w)
					return -1;
				return 0;
			});
			s = "";
			var count = 0;
			for (var i in result) {
				var r = result[i];
				s += "<a href=\"" + root + SEO.articles[r.id].h  + "/" + index + "\">"
					+ "<span>" + SEO.articles[r.id].t + "</span>  "
					+ SEO.articles[r.id].h + " &nbsp;</a>";
				if (++count > 42)
					break;
			}
			div.innerHTML = s;
		}
		else
			div.innerHTML = "<a href='#'><span>No result</span></a>";
	},

	soundex: function (str) {
		// Calculate the soundex key of a string  
		// 
		// version: 1107.2516
		// discuss at: http://phpjs.org/functions/soundex
		// +   original by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
		// +    tweaked by: Jack
		// +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
		// +   bugfixed by: Onno Marsman
		// +      input by: Brett Zamir (http://brett-zamir.me)
		// +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
		// +   original by: Arnout Kazemier (http://www.3rd-Eden.com)
		// +    revised by: Rafał Kukawski (http://blog.kukawski.pl)
		// *     example 1: soundex('Kevin');
		// *     returns 1: 'K150'
		// *     example 2: soundex('Ellery');
		// *     returns 2: 'E460'
		// *     example 3: soundex('Euler');
		// *     returns 3: 'E460'
		str = (str + '').toUpperCase();
		if (!str) {
			return '';
		}
		var sdx = [0, 0, 0, 0],
			m = {
			B: 1,
			  F: 1,
			  P: 1,
			  V: 1,
			   C: 2,
			   G: 2,
			   J: 2,
			   K: 2,
			   Q: 2,
			   S: 2,
			   X: 2,
			   Z: 2,
			   D: 3,
			   T: 3,
			   L: 4,
			   M: 5,
			   N: 5,
			   R: 6
			},
			i = 0,
			j, s = 0,
			c, p;

		while ((c = str.charAt(i++)) && s < 4) {
			if (j = m[c]) {
				if (j !== p) {
					sdx[s++] = p = j;
				}
			} else {
				s += i === 1;
				p = 0;
			}
		}

		sdx[0] = str.charAt(0);
		return sdx.join('');
	}

};

