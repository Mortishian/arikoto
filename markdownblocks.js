/*hammerhead|script|start*/if (typeof window !== 'undefined' && window){window['hammerhead|process-dom-method'] && window['hammerhead|process-dom-method']();if (window.__get$ && typeof __get$ === 'undefined')var __get$Loc = window.__get$Loc,__set$Loc = window.__set$Loc,__set$ = window.__set$,__get$ = window.__get$,__call$ = window.__call$,__get$Eval = window.__get$Eval,__proc$Script = window.__proc$Script,__proc$Html = window.__proc$Html,__get$PostMessage = window.__get$PostMessage,__get$ProxyUrl = window.__get$ProxyUrl,__rest$Array = window.__rest$Array,__rest$Object = window.__rest$Object,__arrayFrom$ = window.__arrayFrom$;} else {if (typeof __get$ === 'undefined')var __get$Loc = function(l){return l},__set$Loc = function(l,v){return l = v},__set$ = function(o,p,v){return o[p] = v},__get$ = function(o,p){return o[p]},__call$ = function(o,p,a){return o[p].apply(o,a)},__get$Eval = function(e){return e},__proc$Script = function(s){return s},__proc$Html = function(h){return h},__get$PostMessage = function(w,p){return arguments.length===1?w.postMessage:p},__get$ProxyUrl = function(u,d){return u},__rest$Array = function(a,i){return Array.prototype.slice.call(a, i)},__rest$Object = function(o,p){var k=Object.keys(o),n={};for(var i=0;i<k.length;++i)if(p.indexOf(k[i])<0)n[k[i]]=o[k[i]];return n},__arrayFrom$ = function(r){if(!r)return r;return!Array.isArray(r)&&"function"==typeof r[Symbol.iterator]?Array.from(r):r};if (typeof importScripts !== "undefined" && /\[native code]/g.test(importScripts.toString())) {var __getWorkerSettings$ = function () {return null};importScripts((location.origin || (location.protocol + "//" + location.host)) + "/worker-hammerhead.js");}}/*hammerhead|script|processing-header-end*/
/**
 * <md-block> custom element
 * @author Lea Verou
 */

/**
 * Pulled from https://md-block.verou.me/md-block.js to bypass certain blocks on certain networks.
 */

let marked = window.marked;
let DOMPurify = window.DOMPurify;
let Prism = window.Prism;

export const URLs = {
	marked: "https://cdn.jsdelivr.net/npm/marked/src/marked.min.js",
	DOMPurify: "https://cdn.jsdelivr.net/npm/dompurify@2.3.3/dist/purify.es.min.js"
}

// Fix indentation
function deIndent(text) {
	let indent = text.match(/^[\r\n]*([\t ]+)/);

	if (indent) {
		indent = indent[1];

		text =  __call$(text,"replace",[RegExp("^"+indent,"gm"),""]) ;
	}

	return text;
}

export class MarkdownElement extends HTMLElement {
	constructor() {
		super();

		this.renderer =  __call$(Object,"assign",[{},this.constructor.renderer]) ;

		for (let property in this.renderer) {
			 __set$(this.renderer,property,__get$(this.renderer,property).bind(this)) ;
		}
	}

	get rendered() {
		return this.getAttribute("rendered");
	}

	get mdContent () {
		return this._mdContent;
	}

	set mdContent (html) {
		this._mdContent = html;
		this._contentFromHTML = false;

		this.render();
	}

	connectedCallback() {
		Object.defineProperty(this, "untrusted", {
			value: this.hasAttribute("untrusted"),
			enumerable: true,
			configurable: false,
			writable: false
		});

		if (this._mdContent === undefined) {
			this._contentFromHTML = true;
			this._mdContent = deIndent(this.innerHTML);
			// https://github.com/markedjs/marked/issues/874#issuecomment-339995375
			// marked expects markdown quotes (>) to be un-escaped, otherwise they won't render correctly
			this._mdContent =  __call$(this._mdContent,"replace",[/&gt;/g,'>']) ;
		}

		this.render();
	}

	async render () {
		if (!this.isConnected || this._mdContent === undefined) {
			return;
		}

		if (!marked) {
			marked = import( __get$ProxyUrl(URLs.marked,"https://md-block.verou.me/") ).then(m => m.marked);
		}

		marked = await marked;

		marked.setOptions({
			gfm: true,
			smartypants: true,
			langPrefix: "language-",
		});

		marked.use({renderer: this.renderer});

		let html = this._parse();

		if (this.untrusted) {
			let mdContent = this._mdContent;
			html = await MarkdownElement.sanitize(html);
			if (this._mdContent !== mdContent) {
				// While we were running this async call, the content changed
				// We don’t want to overwrite with old data. Abort mission!
				return;
			}
		}

		this.innerHTML = html;

		if (!Prism && URLs.Prism && this.querySelector("code")) {
			Prism = import( __get$ProxyUrl(URLs.Prism,"https://md-block.verou.me/") );

			if (URLs.PrismCSS) {
				let link = document.createElement("link");
				link.rel = "stylesheet";
				 __set$(link,"href",URLs.PrismCSS) ;
				document.head.appendChild(link);
			}
		}

		if (Prism) {
			await Prism; // in case it's still loading
			Prism.highlightAllUnder(this);
		}

		if (this.src) {
			this.setAttribute("rendered", this._contentFromHTML? "fallback" : "remote");
		}
		else {
			this.setAttribute("rendered", this._contentFromHTML? "content" : "property");
		}

		// Fire event
		let event = new CustomEvent("md-render", {bubbles: true, composed: true});
		this.dispatchEvent(event);
	}

	static async sanitize(html) {
		if (!DOMPurify) {
			DOMPurify = import( __get$ProxyUrl(URLs.DOMPurify,"https://md-block.verou.me/") ).then(m => m.default);
		}

		DOMPurify = await DOMPurify; // in case it's still loading

		return DOMPurify.sanitize(html);
	}
};

export class MarkdownSpan extends MarkdownElement {
	constructor() {
		super();
	}

	_parse () {
		return marked.parseInline(this._mdContent);
	}

	static renderer = {
		codespan (code) {
			if (this._contentFromHTML) {
				// Inline HTML code needs to be escaped to not be parsed as HTML by the browser
				// This results in marked double-escaping it, so we need to unescape it
				code =  __call$(code,"replace",[/&amp;(?=[lg]t;)/g,"&"]) ;
			}
			else {
				// Remote code may include characters that need to be escaped to be visible in HTML
				code =  __call$(code,"replace",[/</g,"&lt;"]) ;
			}

			return `<code>${code}</code>`;
		}
	}
}

export class MarkdownBlock extends MarkdownElement {
	constructor() {
		super();
	}

	get src() {
		return this._src;
	}

	set src(value) {
		this.setAttribute("src", value);
	}

	get hmin() {
		return this._hmin || 1;
	}

	set hmin(value) {
		this.setAttribute("hmin", value);
	}

	get hlinks() {
		return this._hlinks ?? null;
	}

	set hlinks(value) {
		this.setAttribute("hlinks", value);
	}

	_parse () {
		return marked.parse(this._mdContent);
	}

	static renderer =  window.__call$(Object,"assign",[{heading(text,level,_raw,slugger){level=Math.min(6,level+(this.hmin-1));const id=slugger.slug(text);const hlinks=this.hlinks;let content;if(hlinks===null){content=text;}else{content=`<a href="#${id}" class="anchor">`;if(hlinks===""){content+=text+"</a>";}else{content+=hlinks+"</a>"+text;}}return`
				<h${level} id="${id}">
					${content}
				</h${level}>`;},code(code,language,escaped){if(this._contentFromHTML){code=__call$(code,"replace",[/&amp;(?=[lg]t;)/g,"&"]);}else{code=__call$(code,"replace",[/</g,"&lt;"]);}return`<pre class="language-${language}"><code>${code}</code></pre>`;}},MarkdownSpan.renderer]) ;

	static get observedAttributes() {
		return ["src", "hmin", "hlinks"];
	}

	attributeChangedCallback(name, oldValue, newValue) {
		if (oldValue === newValue) {
			return;
		}

		switch (name) {
			case "src":
				let url;
				try {
					url =  new URL(newValue,__get$Loc(location)) ;
				}
				catch (e) {
					return;
				}

				let prevSrc = this.src;
				this._src = url;

				if (this.src !== prevSrc) {
					fetch(this.src)
					.then(response => {
						if (!response.ok) {
							throw new Error(`Failed to fetch ${this.src}: ${response.status} ${response.statusText}`);
						}

						return response.text();
					})
					.then(text => {
						this.mdContent = text;
					})
					.catch(e => {});
				}

				break;
			case "hmin":
				if (newValue > 0) {
					this._hmin = +newValue;

					this.render();
				}
				break;
			case "hlinks":
				this._hlinks = newValue;
				this.render();
		}
	}
}


customElements.define("md-block", MarkdownBlock);
customElements.define("md-span", MarkdownSpan);

/*hammerhead|script|end*/
