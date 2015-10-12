/** @license Hyphenator 4.1.0 - client side hyphenation for webbrowsers
 *  Copyright (C) 2012  Mathias Nater, Zürich (mathias at mnn dot ch)
 *  Project and Source hosted on http://code.google.com/p/hyphenator/
 * 
 *  This JavaScript code is free software: you can redistribute
 *  it and/or modify it under the terms of the GNU Lesser
 *  General Public License (GNU LGPL) as published by the Free Software
 *  Foundation, either version 3 of the License, or (at your option)
 *  any later version.  The code is distributed WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU GPL for more details.
 *
 *  As additional permission under GNU GPL version 3 section 7, you
 *  may distribute non-source (e.g., minimized or compacted) forms of
 *  that code without the copy of the GNU GPL normally required by
 *  section 4, provided you include this license notice and a URL
 *  through which recipients can access the Corresponding Source.
 *
 * 
 *  Hyphenator.js contains code from Bram Steins hypher.js-Project:
 *  https://github.com/bramstein/Hypher
 *  
 *  Code from this project is marked in the source and belongs 
 *  to the following license:
 *  
 *  Copyright (c) 2011, Bram Stein
 *  All rights reserved.
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *   
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer. 
 *   2. Redistributions in binary form must reproduce the above copyright 
 *      notice, this list of conditions and the following disclaimer in the 
 *      documentation and/or other materials provided with the distribution. 
 *   3. The name of the author may not be used to endorse or promote products 
 *      derived from this software without specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED 
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
 *  EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 *  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  
 */

/* 
 *  Comments are jsdoctoolkit formatted. See http://code.google.com/p/jsdoc-toolkit/
 */

/* The following comment is for JSLint: */
/*global window */
/*jslint browser: true */

/**
 * @constructor
 * @description Provides all functionality to do hyphenation, except the patterns that are loaded
 * externally.
 * @author Mathias Nater, <a href = "mailto:mathias@mnn.ch">mathias@mnn.ch</a>
 * @version 4.1.0
 * @namespace Holds all methods and properties
 * @example
 * &lt;script src = "Hyphenator.js" type = "text/javascript"&gt;&lt;/script&gt;
 * &lt;script type = "text/javascript"&gt;
 *   Hyphenator.run();
 * &lt;/script&gt;
 */
var Hyphenator = (function (window) {
	'use strict';

	/**
	 * @name Hyphenator-contextWindow
	 * @private
	 * @description
	 * contextWindow stores the window for the document to be hyphenated.
	 * If there are frames this will change.
	 * So use contextWindow instead of window!
	 */
	var contextWindow = window,

		/**
		 * @name Hyphenator-supportedLangs
		 * @description
		 * A key-value object that stores supported languages and meta data.
		 * The key is the bcp47 code of the language and the value
		 * is an object containing following informations about the language:
		 * file: filename of the pattern file,
		 * script: script type of the language (e.g. 'latin' for english), this type is abbreviated by an id,
		 * prompt: the sentence prompted to the user, if Hyphenator.js doesn't find a language hint.
		 * @type {Object.<string>, Object>}
		 * @private
		 * @example
		 * Check if language lang is supported:
		 * if (supportedLangs.hasOwnProperty(lang))
		 */
		supportedLangs = (function () {
			var r = {},
				o = function (code, file, script, prompt) {
					r[code] = {'file': file, 'script': script, 'prompt': prompt};
				};

			//latin:0, cyrillic: 1, arabic: 2, armenian:3, bengali: 4, devangari: 5, greek: 6
			//gujarati: 7, kannada: 8, lao: 9, malayalam: 10, oriya: 11, persian: 12, punjabi: 13, tamil: 14, telugu: 15
			//
			//(language code, file name, script, prompt)
			o('be', 'be.js', 1, 'Мова гэтага сайта не можа быць вызначаны аўтаматычна. Калі ласка пакажыце мову:');
			o('ca', 'ca.js', 0, '');
			o('cs', 'cs.js', 0, 'Jazyk této internetové stránky nebyl automaticky rozpoznán. Určete prosím její jazyk:');
			o('da', 'da.js', 0, 'Denne websides sprog kunne ikke bestemmes. Angiv venligst sprog:');
			o('bn', 'bn.js', 4, '');
			o('de', 'de.js', 0, 'Die Sprache dieser Webseite konnte nicht automatisch bestimmt werden. Bitte Sprache angeben:');
			o('el', 'el-monoton.js', 6, '');
			o('el-monoton', 'el-monoton.js', 6, '');
			o('el-polyton', 'el-polyton.js', 6, '');
			o('en', 'en-us.js', 0, 'The language of this website could not be determined automatically. Please indicate the main language:');
			o('en-gb', 'en-gb.js', 0, 'The language of this website could not be determined automatically. Please indicate the main language:');
			o('en-us', 'en-us.js', 0, 'The language of this website could not be determined automatically. Please indicate the main language:');
			o('eo', 'eo.js', 0, 'La lingvo de ĉi tiu retpaĝo ne rekoneblas aŭtomate. Bonvolu indiki ĝian ĉeflingvon:');
			o('es', 'es.js', 0, 'El idioma del sitio no pudo determinarse autom%E1ticamente. Por favor, indique el idioma principal:');
			o('et', 'et.js', 0, 'Veebilehe keele tuvastamine ebaõnnestus, palun valige kasutatud keel:');
			o('fi', 'fi.js', 0, 'Sivun kielt%E4 ei tunnistettu automaattisesti. M%E4%E4rit%E4 sivun p%E4%E4kieli:');
			o('fr', 'fr.js', 0, 'La langue de ce site n%u2019a pas pu %EAtre d%E9termin%E9e automatiquement. Veuillez indiquer une langue, s.v.p.%A0:');
			o('grc', 'grc.js', 6, '');
			o('gu', 'gu.js', 7, '');
			o('hi', 'hi.js', 5, '');
			o('hu', 'hu.js', 0, 'A weboldal nyelvét nem sikerült automatikusan megállapítani. Kérem adja meg a nyelvet:');
			o('hy', 'hy.js', 3, 'Չհաջողվեց հայտնաբերել այս կայքի լեզուն։ Խնդրում ենք նշեք հիմնական լեզուն՝');
			o('it', 'it.js', 0, 'Lingua del sito sconosciuta. Indicare una lingua, per favore:');
			o('kn', 'kn.js', 8, 'ಜಾಲ ತಾಣದ ಭಾಷೆಯನ್ನು ನಿರ್ಧರಿಸಲು ಸಾಧ್ಯವಾಗುತ್ತಿಲ್ಲ. ದಯವಿಟ್ಟು ಮುಖ್ಯ ಭಾಷೆಯನ್ನು ಸೂಚಿಸಿ:');
			o('la', 'la.js', 0, '');
			o('lt', 'lt.js', 0, 'Nepavyko automatiškai nustatyti šios svetainės kalbos. Prašome įvesti kalbą:');
			o('lv', 'lv.js', 0, 'Šīs lapas valodu nevarēja noteikt automātiski. Lūdzu norādiet pamata valodu:');
			o('ml', 'ml.js', 10, 'ഈ വെ%u0D2C%u0D4D%u200Cസൈറ്റിന്റെ ഭാഷ കണ്ടുപിടിയ്ക്കാ%u0D28%u0D4D%u200D കഴിഞ്ഞില്ല. ഭാഷ ഏതാണെന്നു തിരഞ്ഞെടുക്കുക:');
			o('nb', 'nb-no.js', 0, 'Nettstedets språk kunne ikke finnes automatisk. Vennligst oppgi språk:');
			o('no', 'nb-no.js', 0, 'Nettstedets språk kunne ikke finnes automatisk. Vennligst oppgi språk:');
			o('nb-no', 'nb-no.js', 0, 'Nettstedets språk kunne ikke finnes automatisk. Vennligst oppgi språk:');
			o('nl', 'nl.js', 0, 'De taal van deze website kan niet automatisch worden bepaald. Geef de hoofdtaal op:');
			o('or', 'or.js', 11, '');
			o('pa', 'pa.js', 13, '');
			o('pl', 'pl.js', 0, 'Języka tej strony nie można ustalić automatycznie. Proszę wskazać język:');
			o('pt', 'pt.js', 0, 'A língua deste site não pôde ser determinada automaticamente. Por favor indique a língua principal:');
			o('ru', 'ru.js', 1, 'Язык этого сайта не может быть определен автоматически. Пожалуйста укажите язык:');
			o('sk', 'sk.js', 0, '');
			o('sl', 'sl.js', 0, 'Jezika te spletne strani ni bilo mogoče samodejno določiti. Prosim navedite jezik:');
			o('sr-latn', 'sr-latn.js', 0, 'Jezika te spletne strani ni bilo mogoče samodejno določiti. Prosim navedite jezik:');
			o('sv', 'sv.js', 0, 'Spr%E5ket p%E5 den h%E4r webbplatsen kunde inte avg%F6ras automatiskt. V%E4nligen ange:');
			o('ta', 'ta.js', 14, '');
			o('te', 'te.js', 15, '');
			o('tr', 'tr.js', 0, 'Bu web sitesinin dili otomatik olarak tespit edilememiştir. Lütfen dökümanın dilini seçiniz%A0:');
			o('uk', 'uk.js', 1, 'Мова цього веб-сайту не може бути визначена автоматично. Будь ласка, вкажіть головну мову:');

			return r;
		}()),

		/**
		 * @name Hyphenator-languageHint
		 * @description
		 * An automatically generated string to be displayed in a prompt if the language can't be guessed.
		 * The string is generated using the supportedLangs-object.
		 * @see Hyphenator-supportedLangs
		 * @type {string}
		 * @private
		 * @see Hyphenator-autoSetMainLanguage
		 */
		languageHint = (function () {
			var k, r = '';
			for (k in supportedLangs) {
				if (supportedLangs.hasOwnProperty(k)) {
					r += k + ', ';
				}
			}
			r = r.substring(0, r.length - 2);
			return r;
		}()),

		/**
		 * @name Hyphenator-basePath
		 * @description
		 * A string storing the basepath from where Hyphenator.js was loaded.
		 * This is used to load the patternfiles.
		 * The basepath is determined dynamically by searching all script-tags for Hyphenator.js
		 * If the path cannot be determined http://hyphenator.googlecode.com/svn/trunk/ is used as fallback.
		 * @type {string}
		 * @private
		 * @see Hyphenator-loadPatterns
		 */
		basePath = (function () {
			var s = contextWindow.document.getElementsByTagName('script'), i = 0, p, src, t = s[i];
			while (!!t) {
				if (!!t.src) {
					src = t.src;
					p = src.indexOf('Hyphenator.js');
					if (p !== -1) {
						return src.substring(0, p);
					}
				}
				i += 1;
				t = s[i];
			}
			return 'http://hyphenator.googlecode.com/svn/trunk/';
		}()),

		/**
		 * @name Hyphenator-isLocal
		 * @private
		 * @description
		 * isLocal is true, if Hyphenator is loaded from the same domain, as the webpage, but false, if
		 * it's loaded from an external source (i.e. directly from google.code)
		 */
		isLocal = (function () {
			var re = false;
			if (window.location.href.indexOf(basePath) !== -1) {
				re = true;
			}
			return re;
		}()),

		/**
		 * @name Hyphenator-documentLoaded
		 * @private
		 * @description
		 * documentLoaded is true, when the DOM has been loaded. This is set by runOnContentLoaded
		 */
		documentLoaded = false,

		/**
		 * @name Hyphenator-persistentConfig
		 * @private
		 * @description
		 * if persistentConfig is set to true (defaults to false), config options and the state of the 
		 * toggleBox are stored in DOM-storage (according to the storage-setting). So they haven't to be
		 * set for each page.
		 */
		persistentConfig = false,

		/**
		 * @name Hyphenator-doFrames
		 * @private
		 * @description
		 * switch to control if frames/iframes should be hyphenated, too
		 * defaults to false (frames are a bag of hurt!)
		 */
		doFrames = false,

		/**
		 * @name Hyphenator-dontHyphenate
		 * @description
		 * A key-value object containing all html-tags whose content should not be hyphenated
		 * @type {Object.<string,boolean>}
		 * @private
		 * @see Hyphenator-hyphenateElement
		 */
		dontHyphenate = {'script': true, 'code': true, 'pre': true, 'img': true, 'br': true, 'samp': true, 'kbd': true, 'var': true, 'abbr': true, 'acronym': true, 'sub': true, 'sup': true, 'button': true, 'option': true, 'label': true, 'textarea': true, 'input': true, 'math': true, 'svg': true},

		/**
		 * @name Hyphenator-enableCache
		 * @description
		 * A variable to set if caching is enabled or not
		 * @type boolean
		 * @default true
		 * @private
		 * @see Hyphenator.config
		 * @see hyphenateWord
		 */
		enableCache = true,

		/**
		 * @name Hyphenator-storageType
		 * @description
		 * A variable to define what html5-DOM-Storage-Method is used ('none', 'local' or 'session')
		 * @type {string}
		 * @default 'local'
		 * @private
		 * @see Hyphenator.config
		 */
		storageType = 'local',

		/**
		 * @name Hyphenator-storage
		 * @description
		 * An alias to the storage-Method defined in storageType.
		 * Set by Hyphenator.run()
		 * @type {Object|undefined}
		 * @default null
		 * @private
		 * @see Hyphenator.run
		 */
		storage,

		/**
		 * @name Hyphenator-enableReducedPatternSet
		 * @description
		 * A variable to set if storing the used patterns is set
		 * @type boolean
		 * @default false
		 * @private
		 * @see Hyphenator.config
		 * @see hyphenateWord
		 * @see Hyphenator.getRedPatternSet
		 */
		enableReducedPatternSet = false,

		/**
		 * @name Hyphenator-enableRemoteLoading
		 * @description
		 * A variable to set if pattern files should be loaded remotely or not
		 * @type boolean
		 * @default true
		 * @private
		 * @see Hyphenator.config
		 * @see Hyphenator-loadPatterns
		 */
		enableRemoteLoading = true,

		/**
		 * @name Hyphenator-displayToggleBox
		 * @description
		 * A variable to set if the togglebox should be displayed or not
		 * @type boolean
		 * @default false
		 * @private
		 * @see Hyphenator.config
		 * @see Hyphenator-toggleBox
		 */
		displayToggleBox = false,

		/**
		 * @name Hyphenator-onError
		 * @description
		 * A function that can be called upon an error.
		 * @see Hyphenator.config
		 * @type {function(Object)}
		 * @private
		 */
		onError = function (e) {
			window.alert("Hyphenator.js says:\n\nAn Error occurred:\n" + e.message);
		},

		/**
		 * @name Hyphenator-createElem
		 * @description
		 * A function alias to document.createElementNS or document.createElement
		 * @type {function(string, Object)}
		 * @private
		 */
		createElem = function (tagname, context) {
			context = context || contextWindow;
			var el;
			if (window.document.createElementNS) {
				el = context.document.createElementNS('http://www.w3.org/1999/xhtml', tagname);
			} else if (window.document.createElement) {
				el = context.document.createElement(tagname);
			}
			return el;
		},

		/**
		 * @name Hyphenator-css3
		 * @description
		 * A variable to set if css3 hyphenation should be used
		 * @type boolean
		 * @default false
		 * @private
		 * @see Hyphenator.config
		 */
		css3 = false,

		/**
		 * @name Hyphenator-css3_hsupport
		 * @description
		 * A generated object containing information for CSS3-hyphenation support
		 * {
		 *   support: boolean,
		 *   property: <the property name to access hyphen-settings>,
		 *   languages: <an object containing supported languages>
		 * }
		 * @type object
		 * @default undefined
		 * @private
		 * @see Hyphenator-css3_gethsupport
		 */
		css3_h9n,

		/**
		 * @name Hyphenator-css3_gethsupport
		 * @description
		 * This function sets Hyphenator-css3_h9n for the current UA
		 * @type function
		 * @private
		 * @see Hyphenator-css3_h9n
		 */
		css3_gethsupport = function () {
			var s,
				createLangSupportChecker = function (prefix) {
					var testStrings = [
						//latin: 0
						'aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz',
						//cyrillic: 1
						'абвгдеёжзийклмнопрстуфхцчшщъыьэюя',
						//arabic: 2
						'أبتثجحخدذرزسشصضطظعغفقكلمنهوي',
						//armenian: 3
						'աբգդեզէըթժիլխծկհձղճմյնշոչպջռսվտրցւփքօֆ',
						//bengali: 4
						'ঁংঃঅআইঈউঊঋঌএঐওঔকখগঘঙচছজঝঞটঠডঢণতথদধনপফবভমযরলশষসহ়ঽািীুূৃৄেৈোৌ্ৎৗড়ঢ়য়ৠৡৢৣ',
						//devangari: 5
						'ँंःअआइईउऊऋऌएऐओऔकखगघङचछजझञटठडढणतथदधनपफबभमयरलळवशषसहऽािीुूृॄेैोौ्॒॑ॠॡॢॣ',
						//greek: 6
						'αβγδεζηθικλμνξοπρσςτυφχψω',
						//gujarati: 7
						'બહઅઆઇઈઉઊઋૠએઐઓઔાિીુૂૃૄૢૣેૈોૌકખગઘઙચછજઝઞટઠડઢણતથદધનપફસભમયરલળવશષ',
						//kannada: 8
						'ಂಃಅಆಇಈಉಊಋಌಎಏಐಒಓಔಕಖಗಘಙಚಛಜಝಞಟಠಡಢಣತಥದಧನಪಫಬಭಮಯರಱಲಳವಶಷಸಹಽಾಿೀುೂೃೄೆೇೈೊೋೌ್ೕೖೞೠೡ',
						//lao: 9
						'ກຂຄງຈຊຍດຕຖທນບປຜຝພຟມຢຣລວສຫອຮະັາິີຶືຸູົຼເແໂໃໄ່້໊໋ໜໝ',
						//malayalam: 10
						'ംഃഅആഇഈഉഊഋഌഎഏഐഒഓഔകഖഗഘങചഛജഝഞടഠഡഢണതഥദധനപഫബഭമയരറലളഴവശഷസഹാിീുൂൃെേൈൊോൌ്ൗൠൡൺൻർൽൾൿ',
						//oriya: 11
						'ଁଂଃଅଆଇଈଉଊଋଌଏଐଓଔକଖଗଘଙଚଛଜଝଞଟଠଡଢଣତଥଦଧନପଫବଭମଯରଲଳଵଶଷସହାିୀୁୂୃେୈୋୌ୍ୗୠୡ',
						//persian: 12
						'أبتثجحخدذرزسشصضطظعغفقكلمنهوي',
						//punjabi: 13
						'ਁਂਃਅਆਇਈਉਊਏਐਓਔਕਖਗਘਙਚਛਜਝਞਟਠਡਢਣਤਥਦਧਨਪਫਬਭਮਯਰਲਲ਼ਵਸ਼ਸਹਾਿੀੁੂੇੈੋੌ੍ੰੱ',
						//tamil: 14
						'ஃஅஆஇஈஉஊஎஏஐஒஓஔகஙசஜஞடணதநனபமயரறலளழவஷஸஹாிீுூெேைொோௌ்ௗ',
						//telugu: 15
						'ఁంఃఅఆఇఈఉఊఋఌఎఏఐఒఓఔకఖగఘఙచఛజఝఞటఠడఢణతథదధనపఫబభమయరఱలళవశషసహాిీుూృౄెేైొోౌ్ౕౖౠౡ'
					],
						f = function (lang) {
							var shadow,
								computedHeight,
								bdy = window.document.getElementsByTagName('body')[0];

							//create and append shadow-test-element
							shadow = createElem('div', window);
							shadow.id = 'Hyphenator_LanguageChecker';
							shadow.style.width = '5em';
							shadow.style[prefix] = 'auto';
							shadow.style.hyphens = 'auto';
							shadow.style.fontSize = '12px';
							shadow.style.lineHeight = '12px';
							shadow.style.visibility = 'hidden';
							if (supportedLangs.hasOwnProperty(lang)) {
								shadow.lang = lang;
								shadow.style['-webkit-locale'] = "'" + lang + "'";
								shadow.innerHTML = testStrings[supportedLangs[lang].script];
							} else {
								return false;
							}
							bdy.appendChild(shadow);

							//measure its height
							computedHeight = shadow.offsetHeight;

							//remove shadow element
							bdy.removeChild(shadow);

							return (computedHeight > 12) ? true : false;
						};
					return f;
				},
				r = {
					support: false,
					property: '',
					checkLangSupport: function () {}
				};

			if (window.getComputedStyle) {
				s = contextWindow.getComputedStyle(contextWindow.document.getElementsByTagName('body')[0], null);
			} else {
				//ancient Browsers don't support CSS3 anyway
				css3_h9n = r;
				return;
			}

			if (s['-webkit-hyphens'] !== undefined) {
				r.support = true;
				r.property = '-webkit-hyphens';
				r.checkLangSupport = createLangSupportChecker('-webkit-hyphens');
			} else if (s.MozHyphens !== undefined) {
				r.support = true;
				r.property = 'MozHyphens';
				r.checkLangSupport = createLangSupportChecker('MozHyphens');
			} else if (s['-ms-hyphens'] !== undefined) {
				r.support = true;
				r.property = '-ms-hyphens';
				r.checkLangSupport = createLangSupportChecker('-ms-hyphens');
			}
			css3_h9n = r;
		},

		/**
		 * @name Hyphenator-hyphenateClass
		 * @description
		 * A string containing the css-class-name for the hyphenate class
		 * @type {string}
		 * @default 'hyphenate'
		 * @private
		 * @example
		 * &lt;p class = "hyphenate"&gt;Text&lt;/p&gt;
		 * @see Hyphenator.config
		 */
		hyphenateClass = 'hyphenate',

		/**
		 * @name Hyphenator-dontHyphenateClass
		 * @description
		 * A string containing the css-class-name for elements that should not be hyphenated
		 * @type {string}
		 * @default 'donthyphenate'
		 * @private
		 * @example
		 * &lt;p class = "donthyphenate"&gt;Text&lt;/p&gt;
		 * @see Hyphenator.config
		 */
		dontHyphenateClass = 'donthyphenate',

		/**
		 * @name Hyphenator-min
		 * @description
		 * A number wich indicates the minimal length of words to hyphenate.
		 * @type {number}
		 * @default 6
		 * @private
		 * @see Hyphenator.config
		 */
		min = 6,

		/**
		 * @name Hyphenator-orphanControl
		 * @description
		 * Control how the last words of a line are handled:
		 * level 1 (default): last word is hyphenated
		 * level 2: last word is not hyphenated
		 * level 3: last word is not hyphenated and last space is non breaking
		 * @type {number}
		 * @default 1
		 * @private
		 */
		orphanControl = 1,

		/**
		 * @name Hyphenator-isBookmarklet
		 * @description
		 * Indicates if Hyphanetor runs as bookmarklet or not.
		 * @type boolean
		 * @default false
		 * @private
		 */
		isBookmarklet = (function () {
			var loc = null, re = false, jsArray = contextWindow.document.getElementsByTagName('script'), i, l;
			for (i = 0, l = jsArray.length; i < l; i += 1) {
				if (!!jsArray[i].getAttribute('src')) {
					loc = jsArray[i].getAttribute('src');
				}
				if (!!loc && loc.indexOf('Hyphenator.js?bm=true') !== -1) {
					re = true;
				}
			}
			return re;
		}()),

		/**
		 * @name Hyphenator-mainLanguage
		 * @description
		 * The general language of the document. In contrast to {@link Hyphenator-defaultLanguage},
		 * mainLanguage is defined by the client (i.e. by the html or by a prompt).
		 * @type {string|null}
		 * @private
		 * @see Hyphenator-autoSetMainLanguage
		 */
		mainLanguage = null,

		/**
		 * @name Hyphenator-defaultLanguage
		 * @description
		 * The language defined by the developper. This language setting is defined by a config option.
		 * It is overwritten by any html-lang-attribute and only taken in count, when no such attribute can
		 * be found (i.e. just before the prompt).
		 * @type {string|null}
		 * @private
		 * @see Hyphenator-autoSetMainLanguage
		 */
		defaultLanguage = '',


		/**
		 * @name Hyphenator-elements
		 * @description
		 * An object holding all elements that have to be hyphenated. This var is filled by
		 * {@link Hyphenator-gatherDocumentInfos}
		 * @type {Array}
		 * @private
		 */
		elements = (function () {
			var Element = function (element, data) {
				this.element = element;
				this.hyphenated = false;
				this.treated = false; //collected but not hyphenated (dohyphenation is off)
				this.data = data;
			},
				ElementCollection = function () {
					this.count = 0;
					this.hyCount = 0;
					this.list = {};
				};
			ElementCollection.prototype = {
				add: function (el, lang, data) {
					if (!this.list.hasOwnProperty(lang)) {
						this.list[lang] = [];
					}
					this.list[lang].push(new Element(el, data));
					this.count += 1;
				},
				each: function (fn) {
					var k;
					for (k in this.list) {
						if (this.list.hasOwnProperty(k)) {
							if (fn.length === 2) {
								fn(k, this.list[k]);
							} else {
								fn(this.list[k]);
							}
						}
					}
				}
			};
			return new ElementCollection();
		}()),


		/**
		 * @name Hyphenator-exceptions
		 * @description
		 * An object containing exceptions as comma separated strings for each language.
		 * When the language-objects are loaded, their exceptions are processed, copied here and then deleted.
		 * @see Hyphenator-prepareLanguagesObj
		 * @type {Object}
		 * @private
		 */
		exceptions = {},

		/**
		 * @name Hyphenator-docLanguages
		 * @description
		 * An object holding all languages used in the document. This is filled by
		 * {@link Hyphenator-gatherDocumentInfos}
		 * @type {Object}
		 * @private
		 */
		docLanguages = {},

		/**
		 * @name Hyphenator-state
		 * @description
		 * A number that inidcates the current state of the script
		 * 0: not initialized
		 * 1: loading patterns
		 * 2: ready
		 * 3: hyphenation done
		 * 4: hyphenation removed
		 * @type {number}
		 * @private
		 */
		state = 0,

		/**
		 * @name Hyphenator-url
		 * @description
		 * A string containing a RegularExpression to match URL's
		 * @type {string}
		 * @private
		 */
		url = '(\\w*:\/\/)?((\\w*:)?(\\w*)@)?((([\\d]{1,3}\\.){3}([\\d]{1,3}))|((www\\.|[a-zA-Z]\\.)?[a-zA-Z0-9\\-\\.]+\\.([a-z]{2,4})))(:\\d*)?(\/[\\w#!:\\.?\\+=&%@!\\-]*)*',
		//      protocoll     usr     pwd                    ip               or                          host                 tld        port               path

		/**
		 * @name Hyphenator-mail
		 * @description
		 * A string containing a RegularExpression to match mail-adresses
		 * @type {string}
		 * @private
		 */
		mail = '[\\w-\\.]+@[\\w\\.]+',

		/**
		 * @name Hyphenator-urlRE
		 * @description
		 * A RegularExpressions-Object for url- and mail adress matching
		 * @type {RegExp}
		 * @private
		 */
		urlOrMailRE = new RegExp('(' + url + ')|(' + mail + ')', 'i'),

		/**
		 * @name Hyphenator-zeroWidthSpace
		 * @description
		 * A string that holds a char.
		 * Depending on the browser, this is the zero with space or an empty string.
		 * zeroWidthSpace is used to break URLs
		 * @type {string}
		 * @private
		 */
		zeroWidthSpace = (function () {
			var zws, ua = window.navigator.userAgent.toLowerCase();
			zws = String.fromCharCode(8203); //Unicode zero width space
			if (ua.indexOf('msie 6') !== -1) {
				zws = ''; //IE6 doesn't support zws
			}
			if (ua.indexOf('opera') !== -1 && ua.indexOf('version/10.00') !== -1) {
				zws = ''; //opera 10 on XP doesn't support zws
			}
			return zws;
		}()),

		/**
		 * @name Hyphenator-onHyphenationDone
		 * @description
		 * A method to be called, when the last element has been hyphenated
		 * @see Hyphenator.config
		 * @type {function()}
		 * @private
		 */
		onHyphenationDone = function () {},

		/**
		 * @name Hyphenator-selectorFunction
		 * @description
		 * A function that has to return a HTMLNodeList of Elements to be hyphenated.
		 * By default it uses the classname ('hyphenate') to select the elements.
		 * @see Hyphenator.config
		 * @type {function()}
		 * @private
		 */
		selectorFunction = function () {
			var tmp, el = [], i, l;
			if (window.document.getElementsByClassName) {
				el = contextWindow.document.getElementsByClassName(hyphenateClass);
			} else if (window.document.querySelectorAll) {
				el = contextWindow.document.querySelectorAll('.' + hyphenateClass);
			} else {
				tmp = contextWindow.document.getElementsByTagName('*');
				l = tmp.length;
				for (i = 0; i < l; i += 1) {
					if (tmp[i].className.indexOf(hyphenateClass) !== -1 && tmp[i].className.indexOf(dontHyphenateClass) === -1) {
						el.push(tmp[i]);
					}
				}
			}
			return el;
		},

		/**
		 * @name Hyphenator-intermediateState
		 * @description
		 * The value of style.visibility of the text while it is hyphenated.
		 * @see Hyphenator.config
		 * @type {string}
		 * @private
		 */
		intermediateState = 'hidden',

		/**
		 * @name Hyphenator-unhide
		 * @description
		 * How hidden elements unhide: either simultaneous (default: 'wait') or progressively.
		 * 'wait' makes Hyphenator.js to wait until all elements are hyphenated (one redraw)
		 * With 'progressive' Hyphenator.js unhides elements as soon as they are hyphenated.
		 * @see Hyphenator.config
		 * @type {string}
		 * @private
		 */
		unhide = 'wait',

		/**
		 * @name Hyphenator-CSSEditors
		 * @description A container array that holds CSSEdit classes
		 * For each window object one CSSEdit class is inserted
		 * @see Hyphenator-CSSEdit
		 * @type {array}
		 * @private
		 */
		CSSEditors = [],

		/**
		 * @name Hyphenator-CSSEditors
		 * @description A custom class with two public methods: setRule() and clearChanges()
		 * Rather sets style for CSS-classes then for single elements
		 * This is used to hide/unhide elements when they are hyphenated.
		 * @see Hyphenator-gatherDocumentInfos
		 * @type {function ()}
		 * @private
		 */
		CSSEdit = function (w) {
			w = w || window;
			var doc = w.document,
				sheet = doc.styleSheets[doc.styleSheets.length - 1],
				changes = [],
				findRule = function (sel) {
					var sheet, rule, sheets = window.document.styleSheets, rules, i, j;
					for (i = 0; i < sheets.length; i += 1) {
						sheet = sheets[i];
						if (!!sheet.cssRules) {
							rules = sheet.cssRules;
						} else if (!!sheet.rules) {
							// IE < 9
							rules = sheet.rules;
						}
						if (!!rules && !!rules.length) {
							for (j = 0; j < rules.length; j += 1) {
								rule = rules[j];
								if (rule.selectorText === sel) {
									return {
										index: j,
										rule: rule
									};
								}
							}
						}
					}
					return false;
				},
				addRule = function (sel, rulesStr) {
					var i, r;
					if (!!sheet.insertRule) {
						if (!!sheet.cssRules) {
							i = sheet.cssRules.length;
						} else {
							i = 0;
						}
						r = sheet.insertRule(sel + '{' + rulesStr + '}', i);
					} else if (!!sheet.addRule) {
						// IE < 9
						if (!!sheet.rules) {
							i = sheet.rules.length;
						} else {
							i = 0;
						}
						sheet.addRule(sel, rulesStr, i);
						r = i;
					}
					return r;
				},
				removeRule = function (sheet, index) {
					if (sheet.deleteRule) {
						sheet.deleteRule(index);
					} else {
						// IE < 9
						sheet.removeRule(index);
					}
				};

			return {
				setRule: function (sel, rulesString) {
					var i, existingRule, cssText;
					existingRule = findRule(sel);
					if (!!existingRule) {
						if (!!existingRule.rule.cssText) {
							cssText = existingRule.rule.cssText;
						} else {
							// IE < 9
							cssText = existingRule.rule.style.cssText.toLowerCase();
						}
						if (cssText === '.' + hyphenateClass + ' { visibility: hidden; }') {
							//browsers w/o IE < 9 and no additional style defs:
							//add to [changes] for later removal
							changes.push({sheet: existingRule.rule.parentStyleSheet, index: existingRule.index});
						} else if (cssText.indexOf('visibility: hidden') !== -1) {
							// IE < 9 or additional style defs:
							// add new rule
							i = addRule(sel, rulesString);
							//add to [changes] for later removal
							changes.push({sheet: sheet, index: i});
							// clear existing def
							existingRule.rule.style.visibility = '';
						}
					} else {
						i = addRule(sel, rulesString);
						changes.push({sheet: sheet, index: i});
					}
				},
				clearChanges: function () {
					var change = changes.pop();
					while (!!change) {
						removeRule(change.sheet, change.index);
						change = changes.pop();
					}
				}
			};
		},

		/**
		 * @name Hyphenator-hyphen
		 * @description
		 * A string containing the character for in-word-hyphenation
		 * @type {string}
		 * @default the soft hyphen
		 * @private
		 * @see Hyphenator.config
		 */
		hyphen = String.fromCharCode(173),

		/**
		 * @name Hyphenator-urlhyphen
		 * @description
		 * A string containing the character for url/mail-hyphenation
		 * @type {string}
		 * @default the zero width space
		 * @private
		 * @see Hyphenator.config
		 * @see Hyphenator-zeroWidthSpace
		 */
		urlhyphen = zeroWidthSpace,

		/**
		 * @name Hyphenator-safeCopy
		 * @description
		 * Defines wether work-around for copy issues is active or not
		 * Not supported by Opera (no onCopy handler)
		 * @type boolean
		 * @default true
		 * @private
		 * @see Hyphenator.config
		 * @see Hyphenator-registerOnCopy
		 */
		safeCopy = true,

		/*
		 * runOnContentLoaded is based od jQuery.bindReady()
		 * see
		 * jQuery JavaScript Library v1.3.2
		 * http://jquery.com/
		 *
		 * Copyright (c) 2009 John Resig
		 * Dual licensed under the MIT and GPL licenses.
		 * http://docs.jquery.com/License
		 *
		 * Date: 2009-02-19 17:34:21 -0500 (Thu, 19 Feb 2009)
		 * Revision: 6246
		 */
		/**
		 * @name Hyphenator-runOnContentLoaded
		 * @description
		 * A crossbrowser solution for the DOMContentLoaded-Event based on jQuery
		 * <a href = "http://jquery.com/</a>
		 * I added some functionality: e.g. support for frames and iframes…
		 * @param {Object} w the window-object
		 * @param {function()} f the function to call onDOMContentLoaded
		 * @private
		 */
		runOnContentLoaded = function (w, f) {
			var
				toplevel, hyphRunForThis = {},
				add = window.document.addEventListener ? 'addEventListener' : 'attachEvent',
				rem = window.document.addEventListener ? 'removeEventListener' : 'detachEvent',
				pre = window.document.addEventListener ? '' : 'on',

				init = function (context) {
					contextWindow = context || window;
					if (!hyphRunForThis[contextWindow.location.href] && (!documentLoaded || !!contextWindow.frameElement)) {
						documentLoaded = true;
						f();
						hyphRunForThis[contextWindow.location.href] = true;
					}
				},

				doScrollCheck = function () {
					try {
						// If IE is used, use the trick by Diego Perini
						// http://javascript.nwbox.com/IEContentLoaded/
						contextWindow.document.documentElement.doScroll("left");
					} catch (error) {
						window.setTimeout(doScrollCheck, 1);
						return;
					}

					// and execute any waiting functions
					init(window);
				},

				doOnLoad = function () {
					var i, haveAccess, fl = window.frames.length;
					if (doFrames && fl > 0) {
						for (i = 0; i < fl; i += 1) {
							haveAccess = undefined;
							//try catch isn't enough for webkit
							try {
								//opera throws only on document.toString-access
								haveAccess = window.frames[i].document.toString();
							} catch (e) {
								haveAccess = undefined;
							}
							if (!!haveAccess) {
								if (window.frames[i].location.href !== 'about:blank') {
									init(window.frames[i]);
								}
							}
						}
						contextWindow = window;
						f();
						hyphRunForThis[window.location.href] = true;
					} else {
						init(window);
					}
				},

				// Cleanup functions for the document ready method
				DOMContentLoaded = function (e) {
					if (e.type === 'readystatechange' && contextWindow.document.readyState !== 'complete') {
						return;
					}
					contextWindow.document[rem](pre + e.type, DOMContentLoaded, false);
					if (!doFrames && window.frames.length === 0) {
						init(window);
					} /* else {
						//we are in a frameset, so do nothing but wait for onload to fire
						
					}*/
				};

			if (documentLoaded && !hyphRunForThis[w.location.href]) {
				f();
				hyphRunForThis[w.location.href] = true;
				return;
			}

			if (contextWindow.document.readyState === "complete" || contextWindow.document.readyState === "interactive") {
				//Running Hyphenator.js if it has been loaded later
				//Thanks to davenewtron http://code.google.com/p/hyphenator/issues/detail?id=158#c10
				window.setTimeout(doOnLoad, 1);
			} else {
				//registering events
				contextWindow.document[add](pre + "DOMContentLoaded", DOMContentLoaded, false);
				contextWindow.document[add](pre + 'readystatechange', DOMContentLoaded, false);
				window[add](pre + 'load', doOnLoad, false);
				toplevel = false;
				try {
					toplevel = !window.frameElement;
				} catch (e) {}
				if (contextWindow.document.documentElement.doScroll && toplevel) {
					doScrollCheck();
				}
			}
		},

		/**
		 * @name Hyphenator-getLang
		 * @description
		 * Gets the language of an element. If no language is set, it may use the {@link Hyphenator-mainLanguage}.
		 * @param {Object} el The first parameter is an DOM-Element-Object
		 * @param {boolean} fallback The second parameter is a boolean to tell if the function should return the {@link Hyphenator-mainLanguage}
		 * if there's no language found for the element.
		 * @private
		 */
		getLang = function (el, fallback) {
			if (!!el.getAttribute('lang')) {
				return el.getAttribute('lang').toLowerCase();
			}
			// The following doesn't work in IE due to a bug when getAttribute('xml:lang') in a table
			/*if (!!el.getAttribute('xml:lang')) {
				return el.getAttribute('xml:lang').substring(0, 2);
			}*/
			//instead, we have to do this (thanks to borgzor):
			try {
				if (!!el.getAttribute('xml:lang')) {
					return el.getAttribute('xml:lang').toLowerCase();
				}
			} catch (ex) {}
			if (el.tagName.toLowerCase() !== 'html') {
				return getLang(el.parentNode, true);
			}
			if (fallback) {
				return mainLanguage;
			}
			return null;
		},

		/**
		 * @name Hyphenator-autoSetMainLanguage
		 * @description
		 * Retrieves the language of the document from the DOM.
		 * The function looks in the following places:
		 * <ul>
		 * <li>lang-attribute in the html-tag</li>
		 * <li>&lt;meta http-equiv = "content-language" content = "xy" /&gt;</li>
		 * <li>&lt;meta name = "DC.Language" content = "xy" /&gt;</li>
		 * <li>&lt;meta name = "language" content = "xy" /&gt;</li>
		 * </li>
		 * If nothing can be found a prompt using {@link Hyphenator-languageHint} and a prompt-string is displayed.
		 * If the retrieved language is in the object {@link Hyphenator-supportedLangs} it is copied to {@link Hyphenator-mainLanguage}
		 * @private
		 */
		autoSetMainLanguage = function (w) {
			w = w || contextWindow;
			var el = w.document.getElementsByTagName('html')[0],
				m = w.document.getElementsByTagName('meta'),
				i,
				text,
				e,
				ul;
			mainLanguage = getLang(el, false);
			if (!mainLanguage) {
				for (i = 0; i < m.length; i += 1) {
					//<meta http-equiv = "content-language" content="xy">
					if (!!m[i].getAttribute('http-equiv') && (m[i].getAttribute('http-equiv').toLowerCase() === 'content-language')) {
						mainLanguage = m[i].getAttribute('content').toLowerCase();
					}
					//<meta name = "DC.Language" content="xy">
					if (!!m[i].getAttribute('name') && (m[i].getAttribute('name').toLowerCase() === 'dc.language')) {
						mainLanguage = m[i].getAttribute('content').toLowerCase();
					}
					//<meta name = "language" content = "xy">
					if (!!m[i].getAttribute('name') && (m[i].getAttribute('name').toLowerCase() === 'language')) {
						mainLanguage = m[i].getAttribute('content').toLowerCase();
					}
				}
			}
			//get lang for frame from enclosing document
			if (!mainLanguage && doFrames && (!!contextWindow.frameElement)) {
				autoSetMainLanguage(window.parent);
			}
			//fallback to defaultLang if set
			if (!mainLanguage && defaultLanguage !== '') {
				mainLanguage = defaultLanguage;
			}
			//ask user for lang
			if (!mainLanguage) {
				text = '';
				ul = window.navigator.language || window.navigator.userLanguage;
				ul = ul.substring(0, 2);
				if (!!supportedLangs[ul] && supportedLangs[ul].prompt !== '') {
					text = supportedLangs[ul].prompt;
				} else {
					text = supportedLangs.en.prompt;
				}
				text += ' (ISO 639-1)\n\n' + languageHint;
				mainLanguage = window.prompt(window.unescape(text), ul).toLowerCase();
			}
			if (!supportedLangs.hasOwnProperty(mainLanguage)) {
				if (supportedLangs.hasOwnProperty(mainLanguage.split('-')[0])) { //try subtag
					mainLanguage = mainLanguage.split('-')[0];
				} else {
					e = new Error('The language "' + mainLanguage + '" is not yet supported.');
					throw e;
				}
			}
		},

		/**
		 * @name Hyphenator-gatherDocumentInfos
		 * @description
		 * This method runs through the DOM and executes the process()-function on:
		 * - every node returned by the {@link Hyphenator-selectorFunction}.
		 * The process()-function copies the element to the elements-variable, sets its visibility
		 * to intermediateState, retrieves its language and recursivly descends the DOM-tree until
		 * the child-Nodes aren't of type 1
		 * @private
		 */
		gatherDocumentInfos = function () {
			var elToProcess, tmp, i = 0,
				process = function (el, lang) {
					var n, i = 0, hyphenatorSettings = {};

					if (el.lang && typeof (el.lang) === 'string') {
						lang = el.lang.toLowerCase(); //copy attribute-lang to internal lang
					} else if (lang) {
						lang = lang.toLowerCase();
					} else {
						lang = getLang(el, true);
					}
					//if css3-hyphenation is supported: use it!
					if (css3 && css3_h9n.support && !!css3_h9n.checkLangSupport(lang)) {
						el.style[css3_h9n.property] = "auto";
						el.style['-webkit-locale'] = "'" + lang + "'";
					} else {
						if (intermediateState === 'hidden' && unhide === 'progressive') {
							if (!!el.getAttribute('style')) {
								hyphenatorSettings.hasOwnStyle = true;
							} else {
								hyphenatorSettings.hasOwnStyle = false;
							}
							hyphenatorSettings.isHidden = true;
							el.style.visibility = 'hidden';
						}
						if (supportedLangs.hasOwnProperty(lang)) {
							docLanguages[lang] = true;
						} else {
							if (supportedLangs.hasOwnProperty(lang.split('-')[0])) { //try subtag
								lang = lang.split('-')[0];
								hyphenatorSettings.language = lang;
								docLanguages[lang] = true;
							} else if (!isBookmarklet) {
								onError(new Error('Language ' + lang + ' is not yet supported.'));
							}
						}
						elements.add(el, lang, hyphenatorSettings);
					}
					n = el.childNodes[i];
					while (!!n) {
						if (n.nodeType === 1 && !dontHyphenate[n.nodeName.toLowerCase()] &&
								n.className.indexOf(dontHyphenateClass) === -1 && !elToProcess[n]) {
							process(n, lang);
						}
						i += 1;
						n = el.childNodes[i];
					}
				};
			if (css3) {
				css3_gethsupport();
			}
			if (isBookmarklet) {
				elToProcess = contextWindow.document.getElementsByTagName('body')[0];
				process(elToProcess, mainLanguage);
			} else {
				if (!css3 && intermediateState === 'hidden' && unhide === 'wait') {
					CSSEditors.push(new CSSEdit(contextWindow));
					CSSEditors[CSSEditors.length - 1].setRule('.' + hyphenateClass, 'visibility: hidden;');
				}
				elToProcess = selectorFunction();
				tmp = elToProcess[i];
				while (!!tmp) {
					process(tmp, '');
					i += 1;
					tmp = elToProcess[i];
				}
			}
			if (elements.count === 0) {
				//nothing to hyphenate or all hyphenated b css3
				state = 3;
				onHyphenationDone();
			}
		},

		/**
		 * @name Hyphenator-createTrie
		 * @description
		 * converts patterns of the given language in a trie
		 * @private
		 * @param {string} lang the language whose patterns shall be converted
		 */
		convertPatterns = function (lang) {
			/** @license BSD licenced code
			 * The following code is based on code from hypher.js and adapted for Hyphenator.js
			 * Copyright (c) 2011, Bram Stein
			 */
			var size = 0,
				tree = {
					tpoints: []
				},
				patterns,
				pattern,
				i,
				j,
				k,
				patternObject = Hyphenator.languages[lang].patterns,
				c,
				chars,
				points,
				t,
				p,
				codePoint,
				test = 'in3se',
				rf,
				getPoints = (function () {
					//IE<9 doesn't act like other browsers: doesn't preserve the separators
					if (test.split(/\D/).length === 1) {
						rf = function (pattern) {
							pattern = pattern.replace(/\D/gi, ' ');
							return pattern.split(' ');
						};
					} else {
						rf = function (pattern) {
							return pattern.split(/\D/);
						};
					}
					return rf;
				}());

			for (size in patternObject) {
				if (patternObject.hasOwnProperty(size)) {
					patterns = patternObject[size].match(new RegExp('.{1,' + (+size) + '}', 'g'));
					i = 0;
					pattern = patterns[i];
					while (!!pattern) {
						chars = pattern.replace(/[\d]/g, '').split('');
						points = getPoints(pattern);
						t = tree;

						j = 0;
						c = chars[j];
						while (!!c) {
							codePoint = c.charCodeAt(0);

							if (!t[codePoint]) {
								t[codePoint] = {};
							}
							t = t[codePoint];
							j += 1;
							c = chars[j];
						}

						t.tpoints = [];
						for (k = 0; k < points.length; k += 1) {
							p = points[k];
							t.tpoints.push((p === "") ? 0 : p);
						}
						i += 1;
						pattern = patterns[i];
					}
				}
			}
			Hyphenator.languages[lang].patterns = tree;
			/**
			 * end of BSD licenced code from hypher.js
			 */
		},

		/**
		 * @name Hyphenator-recreatePattern
		 * @description
		 * Recreates the pattern for the reducedPatternSet
		 * @private
		 */
		recreatePattern = function (pattern, nodePoints) {
			var r = [], c = pattern.split(''), i;
			for (i = 0; i < nodePoints.length; i += 1) {
				if (nodePoints[i] !== 0) {
					r.push(nodePoints[i]);
				}
				if (c[i]) {
					r.push(c[i]);
				}
			}
			return r.join('');
		},

		/**
		 * @name Hyphenator-convertExceptionsToObject
		 * @description
		 * Converts a list of comma seprated exceptions to an object:
		 * 'Fortran,Hy-phen-a-tion' -> {'Fortran':'Fortran','Hyphenation':'Hy-phen-a-tion'}
		 * @private
		 * @param {string} exc a comma separated string of exceptions (without spaces)
		 */
		convertExceptionsToObject = function (exc) {
			var w = exc.split(', '),
				r = {},
				i,
				l,
				key;
			for (i = 0, l = w.length; i < l; i += 1) {
				key = w[i].replace(/-/g, '');
				if (!r.hasOwnProperty(key)) {
					r[key] = w[i];
				}
			}
			return r;
		},

		/**
		 * @name Hyphenator-loadPatterns
		 * @description
		 * Checks if the requested file is available in the network.
		 * Adds a &lt;script&gt;-Tag to the DOM to load an externeal .js-file containing patterns and settings for the given language.
		 * If the given language is not in the {@link Hyphenator-supportedLangs}-Object it returns.
		 * One may ask why we are not using AJAX to load the patterns. The XMLHttpRequest-Object 
		 * has a same-origin-policy. This makes the Bookmarklet impossible.
		 * @param {string} lang The language to load the patterns for
		 * @private
		 * @see Hyphenator-basePath
		 */
		loadPatterns = function (lang) {
			var url, xhr, head, script;
			if (supportedLangs.hasOwnProperty(lang) && !Hyphenator.languages[lang]) {
				url = basePath + 'patterns/' + supportedLangs[lang].file;
			} else {
				return;
			}
			if (isLocal && !isBookmarklet) {
				//check if 'url' is available:
				xhr = null;
				try {
					// Mozilla, Opera, Safari and Internet Explorer (ab v7)
					xhr = new window.XMLHttpRequest();
				} catch (e) {
					try {
						//IE>=6
						xhr  = new window.ActiveXObject("Microsoft.XMLHTTP");
					} catch (e2) {
						try {
							//IE>=5
							xhr  = new window.ActiveXObject("Msxml2.XMLHTTP");
						} catch (e3) {
							xhr  = null;
						}
					}
				}

				if (xhr) {
					xhr.open('HEAD', url, true);
					xhr.setRequestHeader('Cache-Control', 'no-cache');
					xhr.onreadystatechange = function () {
						if (xhr.readyState === 4) {
							if (xhr.status === 404) {
								onError(new Error('Could not load\n' + url));
								delete docLanguages[lang];
								return;
							}
						}
					};
					xhr.send(null);
				}
			}
			if (createElem) {
				head = window.document.getElementsByTagName('head').item(0);
				script = createElem('script', window);
				script.src = url;
				script.type = 'text/javascript';
				head.appendChild(script);
			}
		},

		/**
		 * @name Hyphenator-prepareLanguagesObj
		 * @description
		 * Adds a cache to each language and converts the exceptions-list to an object.
		 * If storage is active the object is stored there.
		 * @private
		 * @param {string} lang the language ob the lang-obj
		 */
		prepareLanguagesObj = function (lang) {
			var lo = Hyphenator.languages[lang], wrd;
			if (!lo.prepared) {
				if (enableCache) {
					lo.cache = {};
					//Export
					//lo['cache'] = lo.cache;
				}
				if (enableReducedPatternSet) {
					lo.redPatSet = {};
				}
				//add exceptions from the pattern file to the local 'exceptions'-obj
				if (lo.hasOwnProperty('exceptions')) {
					Hyphenator.addExceptions(lang, lo.exceptions);
					delete lo.exceptions;
				}
				//copy global exceptions to the language specific exceptions
				if (exceptions.hasOwnProperty('global')) {
					if (exceptions.hasOwnProperty(lang)) {
						exceptions[lang] += ', ' + exceptions.global;
					} else {
						exceptions[lang] = exceptions.global;
					}
				}
				//move exceptions from the the local 'exceptions'-obj to the 'language'-object
				if (exceptions.hasOwnProperty(lang)) {
					lo.exceptions = convertExceptionsToObject(exceptions[lang]);
					delete exceptions[lang];
				} else {
					lo.exceptions = {};
				}
				convertPatterns(lang);
				wrd = '[\\w' + lo.specialChars + '@' + String.fromCharCode(173) + String.fromCharCode(8204) + '-]{' + min + ',}';
				lo.genRegExp = new RegExp('(' + url + ')|(' + mail + ')|(' + wrd + ')', 'gi');
				lo.prepared = true;
			}
			if (!!storage) {
				try {
					storage.setItem('Hyphenator_' + lang, window.JSON.stringify(lo));
				} catch (e) {
					//onError(e);
				}
			}

		},

		/**
		 * @name Hyphenator-prepare
		 * @description
		 * This funtion prepares the Hyphenator-Object: If RemoteLoading is turned off, it assumes
		 * that the patternfiles are loaded, all conversions are made and the callback is called.
		 * If storage is active the object is retrieved there.
		 * If RemoteLoading is on (default), it loads the pattern files and waits until they are loaded,
		 * by repeatedly checking Hyphenator.languages. If a patterfile is loaded the patterns are
		 * converted to their object style and the lang-object extended.
		 * Finally the callback is called.
		 * @private
		 */
		prepare = function (callback) {
			var lang, interval, tmp1, tmp2;
			if (!enableRemoteLoading) {
				for (lang in Hyphenator.languages) {
					if (Hyphenator.languages.hasOwnProperty(lang)) {
						prepareLanguagesObj(lang);
					}
				}
				state = 2;
				callback('*');
				return;
			}
			// get all languages that are used and preload the patterns
			state = 1;
			for (lang in docLanguages) {
				if (docLanguages.hasOwnProperty(lang)) {
					if (!!storage && storage.getItem('Hyphenator_' + lang)) {
						Hyphenator.languages[lang] = window.JSON.parse(storage.getItem('Hyphenator_' + lang));
						if (exceptions.hasOwnProperty('global')) {
							tmp1 = convertExceptionsToObject(exceptions.global);
							for (tmp2 in tmp1) {
								if (tmp1.hasOwnProperty(tmp2)) {
									Hyphenator.languages[lang].exceptions[tmp2] = tmp1[tmp2];
								}
							}
						}
						//Replace exceptions since they may have been changed:
						if (exceptions.hasOwnProperty(lang)) {
							tmp1 = convertExceptionsToObject(exceptions[lang]);
							for (tmp2 in tmp1) {
								if (tmp1.hasOwnProperty(tmp2)) {
									Hyphenator.languages[lang].exceptions[tmp2] = tmp1[tmp2];
								}
							}
							delete exceptions[lang];
						}
						//Replace genRegExp since it may have been changed:
						tmp1 = '[\\w' + Hyphenator.languages[lang].specialChars + '@' + String.fromCharCode(173) + String.fromCharCode(8204) + '-]{' + min + ',}';
						Hyphenator.languages[lang].genRegExp = new RegExp('(' + url + ')|(' + mail + ')|(' + tmp1 + ')', 'gi');

						delete docLanguages[lang];
						callback(lang);
					} else {
						loadPatterns(lang);
					}
				}
			}
			// else async wait until patterns are loaded, then hyphenate
			interval = window.setInterval(function () {
				var finishedLoading = true, lang;
				for (lang in docLanguages) {
					if (docLanguages.hasOwnProperty(lang)) {
						finishedLoading = false;
						if (!!Hyphenator.languages[lang]) {
							delete docLanguages[lang];
							//do conversion while other patterns are loading:
							prepareLanguagesObj(lang);
							callback(lang);
						}
					}
				}
				if (finishedLoading) {
					window.clearInterval(interval);
					state = 2;
				}
			}, 100);
		},

		/**
		 * @name Hyphenator-switchToggleBox
		 * @description
		 * Creates or hides the toggleBox: a small button to turn off/on hyphenation on a page.
		 * @see Hyphenator.config
		 * @private
		 */
		toggleBox = function () {
			var bdy, myIdAttribute, myTextNode, myClassAttribute,
				text = (Hyphenator.doHyphenation ? 'Hy-phen-a-tion' : 'Hyphenation'),
				myBox = contextWindow.document.getElementById('HyphenatorToggleBox');
			if (!!myBox) {
				myBox.firstChild.data = text;
			} else {
				bdy = contextWindow.document.getElementsByTagName('body')[0];
				myBox = createElem('div', contextWindow);
				myIdAttribute = contextWindow.document.createAttribute('id');
				myIdAttribute.nodeValue = 'HyphenatorToggleBox';
				myClassAttribute = contextWindow.document.createAttribute('class');
				myClassAttribute.nodeValue = dontHyphenateClass;
				myTextNode = contextWindow.document.createTextNode(text);
				myBox.appendChild(myTextNode);
				myBox.setAttributeNode(myIdAttribute);
				myBox.setAttributeNode(myClassAttribute);
				myBox.onclick =  Hyphenator.toggleHyphenation;
				myBox.style.position = 'absolute';
				myBox.style.top = '0px';
				myBox.style.right = '0px';
				myBox.style.margin = '0';
				myBox.style.backgroundColor = '#AAAAAA';
				myBox.style.color = '#FFFFFF';
				myBox.style.font = '6pt Arial';
				myBox.style.letterSpacing = '0.2em';
				myBox.style.padding = '3px';
				myBox.style.cursor = 'pointer';
				myBox.style.WebkitBorderBottomLeftRadius = '4px';
				myBox.style.MozBorderRadiusBottomleft = '4px';
				bdy.appendChild(myBox);
			}
		},


		/**
		 * @name Hyphenator-hyphenateWord
		 * @description
		 * This function is the heart of Hyphenator.js. It returns a hyphenated word.
		 *
		 * If there's already a {@link Hyphenator-hypen} in the word, the word is returned as it is.
		 * If the word is in the exceptions list or in the cache, it is retrieved from it.
		 * If there's a '-' put a zeroWidthSpace after the '-' and hyphenate the parts.
		 * @param {string} lang The language of the word
		 * @param {string} word The word
		 * @returns string The hyphenated word
		 * @public
		 */
		hyphenateWord = function (lang, word) {
			var lo = Hyphenator.languages[lang], parts, l, subst,
				w, characters, origWord, originalCharacters, wordLength, i, j, k, node, points = [],
				characterPoints = [], nodePoints, nodePointsLength, m = Math.max, trie,
				result = [''], pattern, r;
			if (word === '') {
				return '';
			}
			if (word.indexOf(hyphen) !== -1) {
				//word already contains shy; -> leave at it is!
				return word;
			}
			if (enableCache && lo.cache.hasOwnProperty(word)) { //the word is in the cache
				return lo.cache[word];
			}
			if (lo.exceptions.hasOwnProperty(word)) { //the word is in the exceptions list
				return lo.exceptions[word].replace(/-/g, hyphen);
			}
			if (word.indexOf('-') !== -1) {
				//word contains '-' -> hyphenate the parts separated with '-'
				parts = word.split('-');
				for (i = 0, l = parts.length; i < l; i += 1) {
					parts[i] = hyphenateWord(lang, parts[i]);
				}
				return parts.join('-');
			}
			origWord = word;
			w = word = '_' + word + '_';
			if (!!lo.charSubstitution) {
				for (subst in lo.charSubstitution) {
					if (lo.charSubstitution.hasOwnProperty(subst)) {
						w = w.replace(new RegExp(subst, 'g'), lo.charSubstitution[subst]);
					}
				}
			}
			if (origWord.indexOf("'") !== -1) {
				w = w.replace("'", "’"); //replace APOSTROPHE with RIGHT SINGLE QUOTATION MARK (since the latter is used in the patterns)
			}
			/** @license BSD licenced code
			 * The following code is based on code from hypher.js
			 * Copyright (c) 2011, Bram Stein
			 */
			characters = w.toLowerCase().split('');
			originalCharacters = word.split('');
			wordLength = characters.length;
			trie = lo.patterns;
			for (i = 0; i < wordLength; i += 1) {
				points[i] = 0;
				characterPoints[i] = characters[i].charCodeAt(0);
			}
			for (i = 0; i < wordLength; i += 1) {
				pattern = '';
				node = trie;
				for (j = i; j < wordLength; j += 1) {
					node = node[characterPoints[j]];
					if (node) {
						if (enableReducedPatternSet) {
							pattern += String.fromCharCode(characterPoints[j]);
						}
						nodePoints = node.tpoints;
						if (nodePoints) {
							if (enableReducedPatternSet) {
								if (!lo.redPatSet) {
									lo.redPatSet = {};
								}
								lo.redPatSet[pattern] = recreatePattern(pattern, nodePoints);
							}
							for (k = 0, nodePointsLength = nodePoints.length; k < nodePointsLength; k += 1) {
								points[i + k] = m(points[i + k], nodePoints[k]);
							}
						}
					} else {
						break;
					}
				}
			}
			for (i = 1; i < wordLength - 1; i += 1) {
				if (i > lo.leftmin && i < (wordLength - lo.rightmin) && points[i] % 2) {
					result.push(originalCharacters[i]);
				} else {
					result[result.length - 1] += originalCharacters[i];
				}
			}
			r = result.join(hyphen);
			/**
			 * end of BSD licenced code from hypher.js
			 */
			if (enableCache) { //put the word in the cache
				lo.cache[origWord] = r;
			}
			return r;
		},

		/**
		 * @name Hyphenator-hyphenateURL
		 * @description
		 * Puts {@link Hyphenator-urlhyphen} after each no-alphanumeric char that my be in a URL.
		 * @param {string} url to hyphenate
		 * @returns string the hyphenated URL
		 * @public
		 */
		hyphenateURL = function (url) {
			return url.replace(/([:\/\.\?#&_,;!@]+)/gi, '$&' + urlhyphen);
		},

		/**
		 * @name Hyphenator-removeHyphenationFromElement
		 * @description
		 * Removes all hyphens from the element. If there are other elements, the function is
		 * called recursively.
		 * Removing hyphens is usefull if you like to copy text. Some browsers are buggy when the copy hyphenated texts.
		 * @param {Object} el The element where to remove hyphenation.
		 * @public
		 */
		removeHyphenationFromElement = function (el) {
			var h, i = 0, n;
			switch (hyphen) {
			case '|':
				h = '\\|';
				break;
			case '+':
				h = '\\+';
				break;
			case '*':
				h = '\\*';
				break;
			default:
				h = hyphen;
			}
			n = el.childNodes[i];
			while (!!n) {
				if (n.nodeType === 3) {
					n.data = n.data.replace(new RegExp(h, 'g'), '');
					n.data = n.data.replace(new RegExp(zeroWidthSpace, 'g'), '');
				} else if (n.nodeType === 1) {
					removeHyphenationFromElement(n);
				}
				i += 1;
				n = el.childNodes[i];
			}
		},

		/**
		 * @name Hyphenator-oncopyHandler
		 * @description
		 * The function called by registerOnCopy
		 * @private
		 */
		oncopyHandler,

		/**
		 * @name Hyphenator-removeOnCopy
		 * @description
		 * Method to remove copy event handler from the given element
		 * @param object a html object from witch we remove the event
		 * @private
		 */
		removeOnCopy = function (el) {
			var body = el.ownerDocument.getElementsByTagName('body')[0];
			if (!body) {
				return;
			}
			el = el || body;
			if (window.removeEventListener) {
				el.removeEventListener("copy", oncopyHandler, true);
			} else {
				el.detachEvent("oncopy", oncopyHandler);
			}
		},

		/**
		 * @name Hyphenator-registerOnCopy
		 * @description
		 * Huge work-around for browser-inconsistency when it comes to
		 * copying of hyphenated text.
		 * The idea behind this code has been provided by http://github.com/aristus/sweet-justice
		 * sweet-justice is under BSD-License
		 * @param object an HTML element where the copy event will be registered to
		 * @private
		 */
		registerOnCopy = function (el) {
			var body = el.ownerDocument.getElementsByTagName('body')[0],
				shadow,
				selection,
				range,
				rangeShadow,
				restore;
			oncopyHandler = function (e) {
				e = e || window.event;
				var target = e.target || e.srcElement,
					currDoc = target.ownerDocument,
					body = currDoc.getElementsByTagName('body')[0],
					targetWindow = currDoc.defaultView || currDoc.parentWindow;
				if (target.tagName && dontHyphenate[target.tagName.toLowerCase()]) {
					//Safari needs this
					return;
				}
				//create a hidden shadow element
				shadow = currDoc.createElement('div');
				//Moving the element out of the screen doesn't work for IE9 (https://connect.microsoft.com/IE/feedback/details/663981/)
				//shadow.style.overflow = 'hidden';
				//shadow.style.position = 'absolute';
				//shadow.style.top = '-5000px';
				//shadow.style.height = '1px';
				//doing this instead:
				shadow.style.color = window.getComputedStyle ? targetWindow.getComputedStyle(body, null).backgroundColor : '#FFFFFF';
				shadow.style.fontSize = '0px';
				body.appendChild(shadow);
				if (!!window.getSelection) {
					//FF3, Webkit, IE9
					e.stopPropagation();
					selection = targetWindow.getSelection();
					range = selection.getRangeAt(0);
					shadow.appendChild(range.cloneContents());
					removeHyphenationFromElement(shadow);
					selection.selectAllChildren(shadow);
					restore = function () {
						shadow.parentNode.removeChild(shadow);
						selection.removeAllRanges(); //IE9 needs that
						selection.addRange(range);
					};
				} else {
					// IE<9
					e.cancelBubble = true;
					selection = targetWindow.document.selection;
					range = selection.createRange();
					shadow.innerHTML = range.htmlText;
					removeHyphenationFromElement(shadow);
					rangeShadow = body.createTextRange();
					rangeShadow.moveToElementText(shadow);
					rangeShadow.select();
					restore = function () {
						shadow.parentNode.removeChild(shadow);
						if (range.text !== "") {
							range.select();
						}
					};
				}
				window.setTimeout(restore, 0);
			};
			if (!body) {
				return;
			}
			el = el || body;
			if (window.addEventListener) {
				el.addEventListener("copy", oncopyHandler, true);
			} else {
				el.attachEvent("oncopy", oncopyHandler);
			}
		},

		/**
		 * @name Hyphenator-unhideElement
		 * @description
		 * Unhides an element and removes the visibility attr if set by hyphenator
		 * @param Object The Element object from ElementCollection
		 * @private
		 */
		unhideElement = function (elo) {
			var el = elo.element,
				hyphenatorSettings = elo.data;
			el.style.visibility = 'visible';
			elo.data.isHidden = false;
			if (!hyphenatorSettings.hasOwnStyle) {
				el.setAttribute('style', ''); // without this, removeAttribute doesn't work in Safari (thanks to molily)
				el.removeAttribute('style');
			} else {
				if (el.style.removeProperty) {
					el.style.removeProperty('visibility');
				} else if (el.style.removeAttribute) { // IE
					el.style.removeAttribute('visibility');
				}
			}
		},

		/**
		 * @name Hyphenator-checkIfAllDone
		 * @description
		 * Checks if all Elements are hyphenated, unhides them and fires onHyphenationDone()
		 * @private
		 */
		checkIfAllDone = function () {
			var allDone = true, i;
			elements.each(function (ellist) {
				var i, l = ellist.length;
				for (i = 0; i < l; i += 1) {
					allDone = allDone && ellist[i].hyphenated;
				}
			});
			if (allDone) {
				for (i = 0; i < CSSEditors.length; i += 1) {
					CSSEditors[i].clearChanges();
				}
				state = 3;
				onHyphenationDone();
			}
		},


		/**
		 * @name Hyphenator-hyphenateElement
		 * @description
		 * Takes the content of the given element and - if there's text - replaces the words
		 * by hyphenated words. If there's another element, the function is called recursively.
		 * When all words are hyphenated, the visibility of the element is set to 'visible'.
		 * @param {Object} el The element to hyphenate
		 * @private
		 */
		hyphenateElement = function (lang, elo) {
			var hyphenatorSettings = elo.data,
				el = elo.element,
				hyphenate,
				n,
				i,
				r,
				controlOrphans = function (part) {
					var h, r;
					switch (hyphen) {
					case '|':
						h = '\\|';
						break;
					case '+':
						h = '\\+';
						break;
					case '*':
						h = '\\*';
						break;
					default:
						h = hyphen;
					}
					if (orphanControl >= 2) {
						//remove hyphen points from last word
						r = part.split(' ');
						r[1] = r[1].replace(new RegExp(h, 'g'), '');
						r[1] = r[1].replace(new RegExp(zeroWidthSpace, 'g'), '');
						r = r.join(' ');
					}
					if (orphanControl === 3) {
						//replace spaces by non breaking spaces
						r = r.replace(/[ ]+/g, String.fromCharCode(160));
					}
					return r;
				};
			if (Hyphenator.languages.hasOwnProperty(lang)) {
				hyphenate = function (word) {
					if (!Hyphenator.doHyphenation) {
						r = word;
					} else if (urlOrMailRE.test(word)) {
						r = hyphenateURL(word);
					} else {
						r = hyphenateWord(lang, word);
					}
					return r;
				};
				if (safeCopy && (el.tagName.toLowerCase() !== 'body')) {
					registerOnCopy(el);
				}
				i = 0;
				n = el.childNodes[i];
				while (!!n) {
					if (n.nodeType === 3 && n.data.length >= min) { //type 3 = #text -> hyphenate!
						n.data = n.data.replace(Hyphenator.languages[lang].genRegExp, hyphenate);
						if (orphanControl !== 1) {
							n.data = n.data.replace(/[\S]+ [\S]+$/, controlOrphans);
						}
					}
					i += 1;
					n = el.childNodes[i];
				}
			}
			if (hyphenatorSettings.isHidden && intermediateState === 'hidden' && unhide === 'progressive') {
				unhideElement(elo);
			}
			elo.hyphenated = true;
			elements.hyCount += 1;
			if (elements.count <= elements.hyCount) {
				checkIfAllDone();
			}
		},


		/**
		 * @name Hyphenator-hyphenateLanguageElements
		 * @description
		 * Calls hyphenateElement() for all elements of the specified language.
		 * If the language is '*' then all elements are hyphenated.
		 * This is done with a setTimout
		 * to prevent a "long running Script"-alert when hyphenating large pages.
		 * Therefore a tricky bind()-function was necessary.
		 * @private
		 */
		hyphenateLanguageElements = function (lang) {
			function bind(fun, arg1, arg2) {
				return function () {
					return fun(arg1, arg2);
				};
			}
			var i, l;
			if (lang === '*') {
				elements.each(function (lang, ellist) {
					var i, l = ellist.length;
					for (i = 0; i < l; i += 1) {
						window.setTimeout(bind(hyphenateElement, lang, ellist[i]), 0);
					}
				});
			} else {
				if (elements.list.hasOwnProperty(lang)) {
					l = elements.list[lang].length;
					for (i = 0; i < l; i += 1) {
						window.setTimeout(bind(hyphenateElement, lang, elements.list[lang][i]), 0);
					}
				}
			}
		},

		/**
		 * @name Hyphenator-removeHyphenationFromDocument
		 * @description
		 * Does what it says ;-)
		 * @private
		 */
		removeHyphenationFromDocument = function () {
			elements.each(function (ellist) {
				var i, l = ellist.length;
				for (i = 0; i < l; i += 1) {
					removeHyphenationFromElement(ellist[i].element);
					if (safeCopy) {
						removeOnCopy(ellist[i].element);
					}
					ellist[i].hyphenated = false;
				}
			});
			state = 4;
		},

		/**
		 * @name Hyphenator-createStorage
		 * @description
		 * inits the private var storage depending of the setting in storageType
		 * and the supported features of the system.
		 * @private
		 */
		createStorage = function () {
			try {
				if (storageType !== 'none' &&
						window.localStorage !== undefined &&
						window.sessionStorage !== undefined &&
						window.JSON.stringify !== undefined &&
						window.JSON.parse !== undefined) {
					switch (storageType) {
					case 'session':
						storage = window.sessionStorage;
						break;
					case 'local':
						storage = window.localStorage;
						break;
					default:
						storage = undefined;
						break;
					}
				}
			} catch (f) {
				//FF throws an error if DOM.storage.enabled is set to false
			}
		},

		/**
		 * @name Hyphenator-storeConfiguration
		 * @description
		 * Stores the current config-options in DOM-Storage
		 * @private
		 */
		storeConfiguration = function () {
			if (!storage) {
				return;
			}
			var settings = {
				'STORED': true,
				'classname': hyphenateClass,
				'donthyphenateclassname': dontHyphenateClass,
				'minwordlength': min,
				'hyphenchar': hyphen,
				'urlhyphenchar': urlhyphen,
				'togglebox': toggleBox,
				'displaytogglebox': displayToggleBox,
				'remoteloading': enableRemoteLoading,
				'enablecache': enableCache,
				'onhyphenationdonecallback': onHyphenationDone,
				'onerrorhandler': onError,
				'intermediatestate': intermediateState,
				'selectorfunction': selectorFunction,
				'safecopy': safeCopy,
				'doframes': doFrames,
				'storagetype': storageType,
				'orphancontrol': orphanControl,
				'dohyphenation': Hyphenator.doHyphenation,
				'persistentconfig': persistentConfig,
				'defaultlanguage': defaultLanguage
			};
			storage.setItem('Hyphenator_config', window.JSON.stringify(settings));
		},

		/**
		 * @name Hyphenator-restoreConfiguration
		 * @description
		 * Retrieves config-options from DOM-Storage and does configuration accordingly
		 * @private
		 */
		restoreConfiguration = function () {
			var settings;
			if (storage.getItem('Hyphenator_config')) {
				settings = window.JSON.parse(storage.getItem('Hyphenator_config'));
				Hyphenator.config(settings);
			}
		};

	return {

		/**
		 * @name Hyphenator.version
		 * @memberOf Hyphenator
		 * @description
		 * String containing the actual version of Hyphenator.js
		 * [major release].[minor releas].[bugfix release]
		 * major release: new API, new Features, big changes
		 * minor release: new languages, improvements
		 * @public
         */
		version: '4.1.0',

		/**
		 * @name Hyphenator.doHyphenation
		 * @description
		 * If doHyphenation is set to false (defaults to true), hyphenateDocument() isn't called.
		 * All other actions are performed.
		 */
		doHyphenation: true,

		/**
		 * @name Hyphenator.languages
		 * @memberOf Hyphenator
		 * @description
		 * Objects that holds key-value pairs, where key is the language and the value is the
		 * language-object loaded from (and set by) the pattern file.
		 * The language object holds the following members:
		 * <table>
		 * <tr><th>key</th><th>desc></th></tr>
		 * <tr><td>leftmin</td><td>The minimum of chars to remain on the old line</td></tr>
		 * <tr><td>rightmin</td><td>The minimum of chars to go on the new line</td></tr>
		 * <tr><td>shortestPattern</td><td>The shortes pattern (numbers don't count!)</td></tr>
		 * <tr><td>longestPattern</td><td>The longest pattern (numbers don't count!)</td></tr>
		 * <tr><td>specialChars</td><td>Non-ASCII chars in the alphabet.</td></tr>
		 * <tr><td>patterns</td><td>the patterns</td></tr>
		 * </table>
		 * And optionally (or after prepareLanguagesObj() has been called):
		 * <table>
		 * <tr><td>exceptions</td><td>Excpetions for the secified language</td></tr>
		 * </table>
		 * @public
         */
		languages: {},


		/**
		 * @name Hyphenator.config
			 * @description
		 * Config function that takes an object as an argument. The object contains key-value-pairs
		 * containig Hyphenator-settings. This is a shortcut for calling Hyphenator.set...-Methods.
		 * @param {Object} obj <table>
		 * <tr><th>key</th><th>values</th><th>default</th></tr>
		 * <tr><td>classname</td><td>string</td><td>'hyphenate'</td></tr>
		 * <tr><td>donthyphenateclassname</td><td>string</td><td>''</td></tr>
		 * <tr><td>minwordlength</td><td>integer</td><td>6</td></tr>
		 * <tr><td>hyphenchar</td><td>string</td><td>'&amp;shy;'</td></tr>
		 * <tr><td>urlhyphenchar</td><td>string</td><td>'zero with space'</td></tr>
		 * <tr><td>togglebox</td><td>function</td><td>see code</td></tr>
		 * <tr><td>displaytogglebox</td><td>boolean</td><td>false</td></tr>
		 * <tr><td>remoteloading</td><td>boolean</td><td>true</td></tr>
		 * <tr><td>enablecache</td><td>boolean</td><td>true</td></tr>
		 * <tr><td>enablereducedpatternset</td><td>boolean</td><td>false</td></tr>
		 * <tr><td>onhyphenationdonecallback</td><td>function</td><td>empty function</td></tr>
		 * <tr><td>onerrorhandler</td><td>function</td><td>alert(onError)</td></tr>
		 * <tr><td>intermediatestate</td><td>string</td><td>'hidden'</td></tr>
		 * <tr><td>selectorfunction</td><td>function</td><td>[…]</td></tr>
		 * <tr><td>safecopy</td><td>boolean</td><td>true</td></tr>
		 * <tr><td>doframes</td><td>boolean</td><td>false</td></tr>
		 * <tr><td>storagetype</td><td>string</td><td>'none'</td></tr>
		 * </table>
		 * @public
		 * @example &lt;script src = "Hyphenator.js" type = "text/javascript"&gt;&lt;/script&gt;
         * &lt;script type = "text/javascript"&gt;
         *     Hyphenator.config({'minwordlength':4,'hyphenchar':'|'});
         *     Hyphenator.run();
         * &lt;/script&gt;
         */
		config: function (obj) {
			var assert = function (name, type) {
					var r, t;
					t = typeof obj[name];
					if (t === type) {
						r = true;
					} else {
						onError(new Error('Config onError: ' + name + ' must be of type ' + type));
						r = false;
					}
					return r;
				},
				key;

			if (obj.hasOwnProperty('storagetype')) {
				if (assert('storagetype', 'string')) {
					storageType = obj.storagetype;
				}
				if (!storage) {
					createStorage();
				}
			}
			if (!obj.hasOwnProperty('STORED') && storage && obj.hasOwnProperty('persistentconfig') && obj.persistentconfig === true) {
				restoreConfiguration();
			}

			for (key in obj) {
				if (obj.hasOwnProperty(key)) {
					switch (key) {
					case 'STORED':
						break;
					case 'classname':
						if (assert('classname', 'string')) {
							hyphenateClass = obj[key];
						}
						break;
					case 'donthyphenateclassname':
						if (assert('donthyphenateclassname', 'string')) {
							dontHyphenateClass = obj[key];
						}
						break;
					case 'minwordlength':
						if (assert('minwordlength', 'number')) {
							min = obj[key];
						}
						break;
					case 'hyphenchar':
						if (assert('hyphenchar', 'string')) {
							if (obj.hyphenchar === '&shy;') {
								obj.hyphenchar = String.fromCharCode(173);
							}
							hyphen = obj[key];
						}
						break;
					case 'urlhyphenchar':
						if (obj.hasOwnProperty('urlhyphenchar')) {
							if (assert('urlhyphenchar', 'string')) {
								urlhyphen = obj[key];
							}
						}
						break;
					case 'togglebox':
						if (assert('togglebox', 'function')) {
							toggleBox = obj[key];
						}
						break;
					case 'displaytogglebox':
						if (assert('displaytogglebox', 'boolean')) {
							displayToggleBox = obj[key];
						}
						break;
					case 'remoteloading':
						if (assert('remoteloading', 'boolean')) {
							enableRemoteLoading = obj[key];
						}
						break;
					case 'enablecache':
						if (assert('enablecache', 'boolean')) {
							enableCache = obj[key];
						}
						break;
					case 'enablereducedpatternset':
						if (assert('enablereducedpatternset', 'boolean')) {
							enableReducedPatternSet = obj[key];
						}
						break;
					case 'onhyphenationdonecallback':
						if (assert('onhyphenationdonecallback', 'function')) {
							onHyphenationDone = obj[key];
						}
						break;
					case 'onerrorhandler':
						if (assert('onerrorhandler', 'function')) {
							onError = obj[key];
						}
						break;
					case 'intermediatestate':
						if (assert('intermediatestate', 'string')) {
							intermediateState = obj[key];
						}
						break;
					case 'selectorfunction':
						if (assert('selectorfunction', 'function')) {
							selectorFunction = obj[key];
						}
						break;
					case 'safecopy':
						if (assert('safecopy', 'boolean')) {
							safeCopy = obj[key];
						}
						break;
					case 'doframes':
						if (assert('doframes', 'boolean')) {
							doFrames = obj[key];
						}
						break;
					case 'storagetype':
						if (assert('storagetype', 'string')) {
							storageType = obj[key];
						}
						break;
					case 'orphancontrol':
						if (assert('orphancontrol', 'number')) {
							orphanControl = obj[key];
						}
						break;
					case 'dohyphenation':
						if (assert('dohyphenation', 'boolean')) {
							Hyphenator.doHyphenation = obj[key];
						}
						break;
					case 'persistentconfig':
						if (assert('persistentconfig', 'boolean')) {
							persistentConfig = obj[key];
						}
						break;
					case 'defaultlanguage':
						if (assert('defaultlanguage', 'string')) {
							defaultLanguage = obj[key];
						}
						break;
					case 'useCSS3hyphenation':
						if (assert('useCSS3hyphenation', 'boolean')) {
							css3 = obj[key];
						}
						break;
					case 'unhide':
						if (assert('unhide', 'string')) {
							unhide = obj[key];
						}
						break;
					default:
						onError(new Error('Hyphenator.config: property ' + key + ' not known.'));
					}
				}
			}
			if (storage && persistentConfig) {
				storeConfiguration();
			}
		},

		/**
		 * @name Hyphenator.run
			 * @description
		 * Bootstrap function that starts all hyphenation processes when called.
		 * @public
		 * @example &lt;script src = "Hyphenator.js" type = "text/javascript"&gt;&lt;/script&gt;
         * &lt;script type = "text/javascript"&gt;
         *   Hyphenator.run();
         * &lt;/script&gt;
         */
		run: function () {
			var process = function () {
				try {
					if (contextWindow.document.getElementsByTagName('frameset').length > 0) {
						return; //we are in a frameset
					}
					autoSetMainLanguage(undefined);
					gatherDocumentInfos();
					prepare(hyphenateLanguageElements);
					if (displayToggleBox) {
						toggleBox();
					}
				} catch (e) {
					onError(e);
				}
			}, i, haveAccess, fl = window.frames.length;

			if (!storage) {
				createStorage();
			}
			if (!documentLoaded && !isBookmarklet) {
				runOnContentLoaded(window, process);
			}
			if (isBookmarklet || documentLoaded) {
				if (doFrames && fl > 0) {
					for (i = 0; i < fl; i += 1) {
						haveAccess = undefined;
						//try catch isn't enough for webkit
						try {
							//opera throws only on document.toString-access
							haveAccess = window.frames[i].document.toString();
						} catch (e) {
							haveAccess = undefined;
						}
						if (!!haveAccess) {
							contextWindow = window.frames[i];
							process();
						}
					}
				}
				contextWindow = window;
				process();
			}
		},

		/**
		 * @name Hyphenator.addExceptions
			 * @description
		 * Adds the exceptions from the string to the appropriate language in the 
		 * {@link Hyphenator-languages}-object
		 * @param {string} lang The language
		 * @param {string} words A comma separated string of hyphenated words WITH spaces.
		 * @public
		 * @example &lt;script src = "Hyphenator.js" type = "text/javascript"&gt;&lt;/script&gt;
         * &lt;script type = "text/javascript"&gt;
         *   Hyphenator.addExceptions('de','ziem-lich, Wach-stube');
         *   Hyphenator.run();
         * &lt;/script&gt;
         */
		addExceptions: function (lang, words) {
			if (lang === '') {
				lang = 'global';
			}
			if (exceptions.hasOwnProperty(lang)) {
				exceptions[lang] += ", " + words;
			} else {
				exceptions[lang] = words;
			}
		},

		/**
		 * @name Hyphenator.hyphenate
			 * @public
		 * @description
		 * Hyphenates the target. The language patterns must be loaded.
		 * If the target is a string, the hyphenated string is returned,
		 * if it's an object, the values are hyphenated directly.
		 * @param {string|Object} target the target to be hyphenated
		 * @param {string} lang the language of the target
		 * @returns string
		 * @example &lt;script src = "Hyphenator.js" type = "text/javascript"&gt;&lt;/script&gt;
		 * &lt;script src = "patterns/en.js" type = "text/javascript"&gt;&lt;/script&gt;
         * &lt;script type = "text/javascript"&gt;
		 * var t = Hyphenator.hyphenate('Hyphenation', 'en'); //Hy|phen|ation
		 * &lt;/script&gt;
		 */
		hyphenate: function (target, lang) {
			var hyphenate, n, i;
			if (Hyphenator.languages.hasOwnProperty(lang)) {
				if (!Hyphenator.languages[lang].prepared) {
					prepareLanguagesObj(lang);
				}
				hyphenate = function (word) {
					var r;
					if (urlOrMailRE.test(word)) {
						r = hyphenateURL(word);
					} else {
						r = hyphenateWord(lang, word);
					}
					return r;
				};
				if (typeof target === 'object' && !(typeof target === 'string' || target.constructor === String)) {
					i = 0;
					n = target.childNodes[i];
					while (!!n) {
						if (n.nodeType === 3 && n.data.length >= min) { //type 3 = #text -> hyphenate!
							n.data = n.data.replace(Hyphenator.languages[lang].genRegExp, hyphenate);
						} else if (n.nodeType === 1) {
							if (n.lang !== '') {
								Hyphenator.hyphenate(n, n.lang);
							} else {
								Hyphenator.hyphenate(n, lang);
							}
						}
						i += 1;
						n = target.childNodes[i];
					}
				} else if (typeof target === 'string' || target.constructor === String) {
					return target.replace(Hyphenator.languages[lang].genRegExp, hyphenate);
				}
			} else {
				onError(new Error('Language "' + lang + '" is not loaded.'));
			}
		},

		/**
		 * @name Hyphenator.getRedPatternSet
			 * @description
		 * Returns {@link Hyphenator-isBookmarklet}.
		 * @param {string} lang the language patterns are stored for
		 * @returns object {'patk': pat}
		 * @public
         */
		getRedPatternSet: function (lang) {
			return Hyphenator.languages[lang].redPatSet;
		},

		/**
		 * @name Hyphenator.isBookmarklet
			 * @description
		 * Returns {@link Hyphenator-isBookmarklet}.
		 * @returns boolean
		 * @public
         */
		isBookmarklet: function () {
			return isBookmarklet;
		},

		getConfigFromURI: function () {
			/*jslint evil: true*/
			var loc = null, re = {}, jsArray = contextWindow.document.getElementsByTagName('script'), i, j, l, s, gp, option;
			for (i = 0, l = jsArray.length; i < l; i += 1) {
				if (!!jsArray[i].getAttribute('src')) {
					loc = jsArray[i].getAttribute('src');
				}
				if (loc && (loc.indexOf('Hyphenator.js?') !== -1)) {
					s = loc.indexOf('Hyphenator.js?');
					gp = loc.substring(s + 14).split('&');
					for (j = 0; j < gp.length; j += 1) {
						option = gp[j].split('=');
						if (option[0] !== 'bm') {
							if (option[1] === 'true') {
								option[1] = true;
							} else if (option[1] === 'false') {
								option[1] = false;
							} else if (isFinite(option[1])) {
								option[1] = parseInt(option[1], 10);
							}
							if (option[0] === 'onhyphenationdonecallback') {
								option[1] = new Function('', option[1]);
							}
							re[option[0]] = option[1];
						}
					}
					break;
				}
			}
			return re;
		},

		/**
		 * @name Hyphenator.toggleHyphenation
			 * @description
		 * Checks the current state of the ToggleBox and removes or does hyphenation.
		 * @public
         */
		toggleHyphenation: function () {
			if (Hyphenator.doHyphenation) {
				removeHyphenationFromDocument();
				Hyphenator.doHyphenation = false;
				storeConfiguration();
				toggleBox();
			} else {
				hyphenateLanguageElements('*');
				Hyphenator.doHyphenation = true;
				storeConfiguration();
				toggleBox();
			}
		}
	};
}(window));
//Export properties/methods (for google closure compiler)
/* to be moved to external file
Hyphenator['languages'] = Hyphenator.languages;
Hyphenator['config'] = Hyphenator.config;
Hyphenator['run'] = Hyphenator.run;
Hyphenator['addExceptions'] = Hyphenator.addExceptions;
Hyphenator['hyphenate'] = Hyphenator.hyphenate;
Hyphenator['getRedPatternSet'] = Hyphenator.getRedPatternSet;
Hyphenator['isBookmarklet'] = Hyphenator.isBookmarklet;
Hyphenator['getConfigFromURI'] = Hyphenator.getConfigFromURI;
Hyphenator['toggleHyphenation'] = Hyphenator.toggleHyphenation;
window['Hyphenator'] = Hyphenator;
*/
if (Hyphenator.isBookmarklet()) {
	Hyphenator.config({displaytogglebox: true, intermediatestate: 'visible', doframes: true, useCSS3hyphenation: true});
	Hyphenator.config(Hyphenator.getConfigFromURI());
	Hyphenator.run();
}

Hyphenator.languages['ru'] = {
    leftmin: 2,
    rightmin: 2,
    specialChars: unescape("абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯਂ%u200D"),
    patterns: {
        2: "1г1ж1м1п1ф1ц1ш1щъ1ы1ь11э1ю",
        3: "а1ба1да1еа1иа1ка1уа1ча1я1баб1вбг21бе2бжб1л1боб1т2бф2бц2бш2бщ1бы1бь1бя1вав1дв1л2вмвф22вц2вш2вщвъ21вы1вяг2а2ггг2и2гп2гф1дадв21де1дид1л1до2дп1ду2дфд1х2дщ2дъ1ды1дяе1а2ебе1ее1и2еоеэ2е1яжг2ж2м2жф2жц2жъ2зг1зе1зиз1лз1н2зт1зу2зцз1ч2зш1зы1зяи1аи1еи1ии1ки1л2ипи1ри1ти1чи1я2й1йд2йя12кг1ке2кмк2о2кп2кск2у2кф2кц2кш1кьк2ю2лб1ли2лм1ло2лпл1т2лцл1чл2ю1ля2мж2мм2мп2мф2мц2мщ2мэм2ю1на2нг1не1нин1л1но1нун1х2нц2нш2нщ1нын2э1няо1вог2о1ео3и2ойо1ко1т2оюо1япе1пх22пц2пш2пщ2рг2рз2рм2рп2рф2рх2рц2рш2рщ2рър2ю1сасг2с1зс2мс1н1сосп21ср1сусч2сш21сы1ся2тг2тжтм22тф4тц2тщ2тъ2ть2тэт2юу1ау1еу1иу1оу1у2уэу1я2фгф4и2фм2фф1ха2хг1хе1хи1хохп22хшца12цгци12цм3цу2цц3цыцю11чач1в1чеч2ж1чи1чм3чо1чуч2хш2в2шм2шфш1х2шц2шь2щмъю2ъя2ые2ыи2ыу2ьб2ь2еь2оь2юь2яэ1в2эгэ2мэ2нэ2пэс1э2фэх2э2цэя2ю1аю1бю1вю1ею1ию1к2юмю1хю1чю1яя1ея1ия1кя1ля1уа1ё1бё1дёе1ё2ёб1зёи1ё1кё1нёо1ёпё1у1ё1чёь2ёю1ё",
        4: "_аи2_ау2_ии2_ио2_ис3_ль2_оз4_ск2_ст2_уб2_уд2_уе2_ук2_уо3_уп2_ус2_ую2_юс14а3ааа2паа2раа2ца3буав1ва1веа1виа1воа2вта1вуа2вх2агаа2гд2агоа3гу2адва2длад2цае2ла2епае2сазв2азг2аз1ра2ихак1в1аккак2лак1са1лаа1леа3лиа1луа1лыа1лю2амаамб42амоа2мчан1ра1нь2а1оао2дао2као2рао2с2апоа1раа1реа1риа1роа1руар1ха1рыа1рюа1ряа1таа1тиа1тоа1туат2ха1тыа1тюа1тяа2убау2дау2хау2чауэ1ах2аах3с2ачаа2члач1та2шла2эрая2бая2вая2зба1зба2о2б1ббвы22б1д3бев3бее3бец2бещб1з21б2и3биаби2б2биж3бик3биоби1х2б3к2блы2бля2б3н3бот2бр_2брсб1ру2брьб1ряб3скбс4л1б2убу1с2б1х2б1чбы2с2бь_2бьс2бьтбэ1р3б2ю3вагва1звах13вац3вая2в1бв1вив1вр2вг21вев3вег1вее1вез1вес1вец1вею1веявзг2взд2взъ21визви1овиу3ви2ф2в1квк2лв2ла2вли2влю2вля1вме2в1нвно1в3нывов21вод1воквоп21вох1вою2вп22вр_1врюв1ряв1т21вуаву3г2вуиву3п1вхо2в1чвып2вых22вь_1вье2вьс2вьт1вью1вья1в2юга1зга2у2г3бгба2г1ви3ге_2г3ж2г1з2г1кг1ле2г3мг3няго1з3гойг2ол3гою2гр_2грюг4саг4сб2г3тгу1вгу1с2г1ч2г3ш2г3эда1зда2о2д1бд1ве1двид3вкд1вл2двя2дг23дез2дж_2джсдип2диу3ди1х2д1к2д1нд3надо1здоп2до1с2др_д1ред1рид1рыд1рядск22д1тду3гду2оду1х2дцу2дцы2д1ч2дыг2дыд2дыт2дыщ2дь_1дье2дьк2дьт1дью1дья1д2юеа2деа2зе1вее1вие1вое2вте1вуе1вхе1вьег2дед2жее2хе2жг2ежее3зее3зяеи2деи2меи2ое1каек2зе1кие1куе1лае1лее1луе1лые1люе3ляе2мче3наенс2е1нэе1оде2оие2омеоп2еос22епее2пле4пн2епое4пте1рае2рве1рее1рие1рое1руе1рые1рюе1ряе1сге1ск2есле3со2еспе1сте1тае1тие1тоет1ре1туе1тые1тюе1тяе1у22еубеуз2еф2л2ецве1чле2шлею2гея2зжа1з2ж1в2жгаж2гиж2гу2ж1дж2диждо3жду14ждь3жев2жжаж2жежи1о2ж1к2ж1лж3ма2ж1нжно1жоу32жп2жпо1ж2ру2ж1с2ж1ч2жь_2жьс2жьт1за1заа2заб2заг4зап2зас2зат2зау2зах2зая2з1б2з1вез1виз1воз1вр1звуз1вьз3га2зж2з3з23зи_3зис3зич2з1кзко12зм2з3мн2зне2зно2зну1зов1зое1зои1зон1зоозос21зохзош21зоэ1зоюз1раз1роз1руз2рюз1ря2з1сз1ти3зу_зу1в3зуе2зупзы2з2зыщ1зье1зьи1зью3зья1з2юи2аби2авиаг2иао2и2апи2аф2и1би1вии1ву2ивыиг2ди3ге2игли2гни1двид1ри1дьие2гие2дие2ри1зоиз1рийс2и3кои3куилп2и2ль2имаи3мии2мчинд21инжинс21инф1инъи1оби2оги1оди1ози1они1ори1ошип3ни2рви2рж1ирри2сби2сдис1ки4сси1сти2тми1у2иу3пиф1ли2фри1хуи2штию4лию2нию2тия2дйно1йп2лй2сбй2снй2сшй2тмй2хм1кавк2ад1кае1кай1кам1кан1кат1ках1каю2к1бк1вик1ву2к1дкда22кеа2к1з1кивки1о1киткк3ск3лы2кль1клю2к1н1ков1код1коз1кос1кош2кр_кс3гкс3мк3сок3су2к1ткт2р3куе1куй3кум1кур1кут2к1ч1ла_1лаел2аклау11лаял1брл1вел1вил1вол1вул2гллго11ле_1лен1лехл1зо2л1клк2в2л1лл2льллю12л1нлс3б1лу_лу1влу3г1луе1лунлу1с1лую1лы_1лые1лыж1лый1лым4ль_2льд3лье3льи2льк2льм2льн3льо2льт2льц2льч3лью3лья1лю_1люж3ля_2ляд3лям3лях3магма2умаф23мач4м1бм3бимб2л2мг22м1д3мкн2м1л2м1н3мод3мон3моп3мофмп2л2мрим1ры2м1смс2кмс2н2м1тмфи32м1х2мш2мым1мы2с2мь_2мьсмью1мэ1рмя1р2нач2нащ3ная2н1внг4лнг2р2н1днд2жн2длн2дцнег23недне3енеи23неунея23нийниу3ни1х3ниц3нищ2н1кнк2внк2лнк1с2н1нноб2ноэ2н3п2н1ро2н1сн2сгн2слн2сн2н1тн2тмну1х3ную2нф2нхо12н1чн2члнш2т3ны_2нь_1нье1ньи2ньк1ньо2ньс2ньт2ньч1нью1нья1н2юо3авоап12оба1обмоб1р1объ2обьов2то2вхо1дьое1бое2дое1оое2цо1зооие3ои2зои2мои2оойс2ок2в1окто3лао1лео3лоо1луо1лыо1люо3ляо3ма2омеом2чо2мьо3наонд2о1нронс2о1о2о2оло2офо1рао1рео1рио1ро2орцо1рыо1рюо1ряос2бо1ст2осхотв21отг1отдо3тио2тм1отхо1у2оуп2о3фе2охио1хро1хуо2цооч2ло1чтош2тоя2воя2доя2зпа3ф2п1дпе2з4п3к2пл_2п1нп3нап3ны3пой2пп22пр_при12прсп2руп3со2п1тп3ту3пуб2пф24п3ч2пь_2пьтп2ю11ра_раа21раю1рая2р1бр1вир1вор1вьр2гвр2гнрг2р2р1дрд2жр2длр2дц1ре_р1зори3ариб2р2ин1риу1риц1риш2р1кр2кврк1ср2льрлю1р3ляр2мч2р1н1ро_1роу2р1р4р1срс2кр2снрс2п2р1тр2тм1ру_1рулрф2лр2хврх1лрх1рр2цв2р1чр2члр2чмрш2т1ры_1рыб2рыз1рым2рь_1рье1рьи2рьк2рьс2рьт1рью1рьярэ1л1рю_1рюс1ряю1сб2с2бы2сбю1с2вс2гис2гнс2го1сд2с2дас2дес3дис2до1с2е1с2ж1с2и3сизси1х4ск_ск2л2скнск2рск1с2сль2снос2овсо1дс3пн2спь2ср_2с1сс2сбсс2лс2снсс2псст2сс2ч2ст_2стбс2те1сти2стк2стм2стн2стп2стс2стф2стц1стыс4тьсу2бсу1всу2зсу1хс1х22сца2сцо1счас1чл2счос3шн1съ2сы2зсы2с2сь_1сье2ськ2сьт1сью1сьясэ1рс2эс1с2юсю1с2сяз1тагт2ан1тас1тащ2тв_2тви2тву2твы2твя2т1д1т2ете1д2т1зтии2тик23ткн2т1лт2льт3мщ2т1нто1д1тощ2тп22трб2трв2трг2трд2трм2трн2трп2трр2трф2трщ2трът1рыт2сб2т1тт2тм1тущ2т1х2т1ч2тш24ть_3тье3тьить2м4тьттью1тю1т1тяг1тяж1тяпу2асуб1ру1виув2лу1воу1вуу2гву2глу2гнуд2ву3дууе2дуе2луе1суе2ху2жжу1зоу1каук1ву1киу1коу1лау1леу1луу1люу2мчу3нау1ньуо2буо2вуо2куо2пуо2суо2фу2плу1рау1реу1риу1роу3руу1рыу1рюу1ряу1сгус2лу1сму2снус2пус3су1сф2усцу2счу2сьу1тау1тиу1тоу1туу1ты1утюу1тяууг2уу2су3фиуф1лу2фру2хвух1лух1р1учру1чьу3шеу3шиу2шлу2шпуя2зфа2х3фашфаэ12ф1б2ф1в2ф1дфи2жфи1о3фит2ф1кф2лаф2лиф2ло2ф1н3фон3фотф1риф1роф1ру2ф3с2ф1тф2тм2фуф2ф1ч2фш22фь_ф2ю1ха2дхао32х1б1х2вх3вых3д2хео3х1з2хие2х1к2х1лу2х1нхоп2хоф2хоя2х1рых1ря2х1т1ху_2хуе2хуй1хун1хус1хуш2хуюх1х2хью13ца_3цам3цах2ц1бц2ве2цвы2ц1дце1зце1кце1т2ц1зцип2циу32ц1л2ц1н2цп22ц1р2ц1с2ц1тцы2п2ч1б2ч1дче1очжо23чик3чиц2ч1кч2ле2чли2чма2чмеч2мо2ч1н2ч1сч2те2чтм3чук2ч1ч2чь_1чье1чьи2чьс2чьт1чью1чья2ш1бше1кш1лыш2лю2ш1н4шниш2п2ш3пр2ш1р2ш1сш1ти2штс2ш1ч4шь_3шье3шьи3шью3шьяш2ю1щеи2ще1сще1хщеш22щ1н2щ1р2щь_ъе2гъе2дъе2лъе2съя3ны2блы3гаы3гиыг2лы2гны2длыз2ды2злы2зныиг1ык2лык1сы2льы2мчы3поыр2вы3саы3сеы2сны3соыс2пы2схыс2чы2сшыт2ры3шьь2вяь2дцье1кь2знь2и1ь2кльмо1ьс2кь2снь2тмьхо2ь2щаь2щеь2щуья1вэв1рэд1рэк1лэкс1э3маэ3ньэо2зэ1реэ1риэ1руэ1рыэск2эс3мэ2соэ2теэхо3ю2бвю2блю1дьюз2гю1зою1лаю1лею2лию1люю2мчю2нью1о1ю1раю1рею1рию1рою1рую1рыю1тию1тою1тую1тыю2щья2бря1воя1вуя2гняд1вяд1ря1зояк1ся2лья2мья3наянс2я1рая1рия1роя1рьяс1кяс1ляс2тя1таят3вя3тия1тоя1туя1тыя1тяях1ля1хуяце1я2шл2яю_2я1я6зь_й2кь6тр_а1вёа1лёа1рё1веё1вёз1вёс1вмё1вьёг1лёд1вёд1рё1дьёе1вё2ежёе3зёе1лё2епёе1рёё1веё1воё1ву2ёжеё3зеё1каё1киё1куё1лаё1леё1луё1лыё2мчё3наёнс22ёпеё2плё4пн2ёпоё4птё1раё1реё1риё1роё1руё1рыё1ск2ёслё3соё1стё1таё1тиё1тоёт1рё1туё1тыё1тюё1тя3жёвж2жёз1вё2знё3зуё1каё3куё1лён1лёх1луё3льёне3ё1ньёо1лё2омёо1рёо3фё1рьёс2дё1с2ёс2тё1сьё1т2ё3тьё_уё2у1лёу1рёу3шёц2вёч2тё1чьё3шьёы3сёь2щё_не88не_8бъ_8въ_8гъ_8дъ_8жъ_8зъ_8къ_8лъ_8мъ_8нъ_8пъ_8ръ_8съ_8тъ_8фъ_8хъ_8цъ_8чъ_8шъ_8щъ_",
        5: "_аб1р_ади2_ак1р_би2о_го2ф_дек2_ди1о_до3п_епи3_за3п_иг1р_изг2_из3н_ик1р_ле2о_на1в_на3т_не3т_ово1_ог3н_ос2п_от1в_ри2ч_ро2х_су2ж_тиа3_ти2г_ти2о_ум2ч_ур2в_ут2ра3блааб2люаб1риав3зоави2ааво1са2вотав1раав2сеа2глеаг2лиа2двеад2жиад1роаду3ча2дынае2гоае2диае2реаз1влаз1драз1обаи2г1аи3глако3т2акриа3лаг2алекало1залу2ша2льщ2аметамои2а2нафан2спанс1уаост1а3плаап2ра1аргуар2жа2ас1кас3миас3ноа1сьи1атакат3ваат1виат1ву2атезато2шат1риа1тьеа3тьюа3тьяау3доа2улеаут1рау3чьа2ф1лахми2аэ2лиаю1таба2бвба2дрба3зубалю1бас3мба1стба1трбе2глбе2гн3бе2збе3зибез3нбез1рбес3пби2обби2одби2онби2орби2тв1благб2ланб3ленб2луд2б2льб2людб2люеб2люлбо3вшбо2гдбо1з2бо2мчбо3мшбону1бо1рубо2сабо1скбо2твбот2рбоя2рб3рабб2равб2ран1брасб1рахб1рейб1рекб2ремб2рехб2ридб1рол1б2рю2б1с2бук1лбы2г1быс1кбыст1бю1тава2брвадь2ванс2ва1ства1трв2дохвед1рве3ду3везе3везлвез2у1вей_ве2п12вердвет3р1в2з2взо1бви2азви2акви2арвиа1тви3афви2гвви2гл1винт1винчв2левв2лекв2летв2лечв2лияв2люб4в3нав2несв3ну_во1б2во3вкво1двво1дрво2ерво2жжво3м2во1ру2ворц2ворьвос1кво1смво1снвот2рво1хл2вра_в2равв1рас2врац2вре_1вридв1риив1рикв1рилв1рисв1рит2в1ро2в1ры2в1с23все33в2сп3в2сювто3ш1ву1з2ву1кву1с2вух3вву1члвы3г2вы3знвы3т21вьин1в2э1г3дан2г3диге2б1гено1ге2обге2одге1орги2блги3брги2грги1слгист22гла_г2лавг1лай2глаяг2лет2гли_г2лин2гло_2глов2глог2глое2глой2глою2глую2г1лыг2ляж2глякг2навг2нанг3не_г2невг3ненг3непг3несг2нирг2ноег2ноиг2носго1б2го2влго2злгоз2нгоиг2гоми2го2сдго1сн2готдгоу3тго1члг1раег1райг1рарг1регг1рекг1рецг1рикг1рилг1ринг1рисг1ричг1ровг2розг1рокг1ронг1ропг1ротг1рофгру2пг1рывг1ряег1рялг1рят2г3с2да2б1да2грдат1р2двиз2дводд1воз2д1д23деврде2зиде2зудеио2де1кл3демеде2оддео3пде3плдес2кде2срде1хлд2жамд2ж3м2д1з2ди2аддиа2зди2арди2асди2обди2ордио1сди2пиди3птди3фрд2лев2д3м2днеа2днос24д3ныдо2блдов2лдо1д2до3дндоз2ндои2р2доктдо3плдос2п2дотд2дотл2дотъдо3ть3дохлдо2щуд1рабд1рард1рахд1рачд2раюд2реб2дрезд2релд2ремд2рий2дринд2рипд2рихд1родд1роед1ройд1ролд1ронд1росд1ротд1роюд1руб1друг1дружд1румд1рую2дрывд2рябд2рях2д1с2дс3кндуб3р2д1удду2дадуп1лдус1кд1услду1стду2чидуэ1т2д3це2д3ш2дъе2м2дымедь3яреади3еа3доеат1реба2се1браеб1рие1броеб1ры2евер2еволев1риев2хое2глее2глие2глоег2наег2но2ег2ред1вое1джее2дохе1друе2дуге2дусе2дынее2гиее1с2ее2стеж3дие2ж1резау3езд1реззу3е3зитез1обе1зомез1опез1отез1ошез2ряез1упез1усеи2г1еис1лека2б2е1ко2е1крек2роек1скеми3кемо1с2емуж2емыс2е1нрен3ш2е1о2бео3даео2деео2дое1о2жео3кле1ол_е1олаео3лие1олке1олые1олье1он_е2онае2ониео3ное1онсе1опеео2прео4пуео1ске1осме1оснео3схе1отле1о2че1о2щепат2е3плаеп1луе3плые4п3сер1вее3ре_ере3перо2б2еролер3ске3с2аес2бае2скее1слуе1слые1с4ме2спуе2стле3стует1веет1вие1тво2етечето1сет2ряе1тьее3тьюе3тьяеуб3реф1рееха2тех1обех1реех1ружа2блжа2бржат1в2ж1б23ж2глж2дакж2дачж2деп4ж2дл3ж2дяже3д2же1клже1о2же3п2же1с2же3ск2жжев2ж1з22жирр2ж3мо2ж1обжоу1сз1авуз1адрзае2дзае2хза3з2з1акт3з2анза3назанс2зар2взар2жзаст2за3ткзач2тза3ш2з2вавз2ван2зваяз2везз3в2кз1вла2зволз2глизг2наз2гнуз1д2вз2дешздож3зе2б1зе2евзе2од3зий_з1интзи2оззи1опзиу3мз2лащз2лобз2лопз2лорз2лющ2зна_з2навз2наез2найз2накз2нанз2натз2наю2зная2з3ни2з3ныз2обезо2бизо2глзо1дрзо1з21зой_1зок_з1окс1зол2зо1лгзо1лжзо3м21зом_2зомн2зонрзо2осзо2паз2оплз2опрз1оргз1оснзо1спзо2твз2отез1откз2отозо2шиз2ракзра2сз2рачз2ренз1ресз2риш2зуве2зу2г2зу1к3зумезу2прз1урбзъе2м2зыме2зымчи2агри2адеи2адииа2муи3анаианд2и3атуи2а1хиа2цеи2б1р2иваж2и1веи2в3з2и1вои1в2ри3в2сив2хои2глеи2глиигни3иг1роиг1руиг1рыи2дейи1д2жие3деие2зуи3ениие1о2иепи1и3ж2диз1в21из1дизо2ои3к2аик2ваи2квии2кляик1роик1скильт2имои2им3пли2м1рим2чаино1с1инсп1инсуио2боио2врио2деио3зои1окси1олеи1опти3ораио1руио2саи1отаи1отки1отсиоуг2ио2хоипат2ип2ляириу3ис3кеис3киис1лыис3меис3муис3нои2стли1сьиита2вит3ваит1виит1вуи2т1ри3тьюи3тьяиф2люиха3ди3х2оихо3ких1реих1рииш2лии2шлыию3тай2д3вй2о1сйо2трй3скайс2кейс4мой2с3фйх2с3ка2блказ3нка1зо1кал_1кало1калс3к2аска1стк2вакк2васк2вашк2возке2глкед1ркиос1ки2пл2к1к22клемк3ленк1леок2ликк2линк3лияк2лозк3ломкло3т1клук2кля_2клям2кляхк2ноп3ковако1др3конскоп2рко1руко1сккос3мко1сп1котнко2фрк1релкре1ок1реч1крибк1ридк2ризк2ритк1рихк1роак1робк2роек1рокк1роок1рорк1роск1рофк1рохк1роэкру1ск1рядк2с3вк2с3дк2сибк1скикс1клк1скокс3тек1стокс1трк1стукта2ккто1ску1ве1кулякуп1лку3рокус1кку1стку3ть1куче1куют3кующ2к1х22лабела2бл2лагола2грла1золак2р1лам_ла2усла2фр1ла1х2л3д2ле1влле1джле3доле1зрлек1л2лемнле2сбле2скле1твле1хрлиа2м3ливо3ливылиг2ллие3рли2кв2лимплио1сли2пллис3мли2твлиу3мли1хлли1хрл2к1ллни2ело2блло2влло1др2лоенло1звло2клло2рвло1рулос1к2лотдлот2рло2шл2л1с2лу1брлу1знлу1крлуо2длу3ть2л3ф22л1х2л2х3в1лых_2льск1льща1льще1льщу1люсьлю1таля1ви3ляво3лявыля1реля1рума2взма2гнма2дрма2дьма1зомас3лма2чтм3бля2м3в23м2глмеан2ме2егме2елме1зомеч1т2м1з2ми1зв2миздми1знми2крми2озми2ор2м1к2м2леем2лел4м3намне1д4мноем2нож4мной4мномм2нор4мноюм2нут4м3нымо1б2мо3влмо1дрмо2жжмо1звмо1зрмо3м2мо1румо1сммо1сн3мотим1раб2м1рому1стмут1рму3тьм2чавм2чалм2читм2чиш3м2щемы2мрмя1стнаб2рнаг2нна3ждна1з2на2илна2ин4наккнап2лна1с2на1твна1х2наэ1р2н1б2н2г1внги2онго1сн2дакн2д1внде3знде2сн3д2знд2рен2дрянд2спне1б22невннед2оне3дунее2дне1звне1знне1зоне1зрне1клне2олне3п2нес2кнест2не2фрне1хрне3шк2н1з2нзо1сни3б2ни2енни2клнила2ни1слнис3пнкоб2но1брно2влно1двно1дрно2ерно1звно2здно1зрно3кн3номеном3шно2рвно1руно2сч2нотдно3ф22н1ре2н1рин2с3внс2кен3слан2с3мнст2рнсу2рн2с3фн2съ3н2т1внт2рант2рент2рунт2рынут1рня1ви2о1а22обиоо1блюобо2с2обото3влаов3ноов2се2о3гео3гря2одано3де_о2дыно2дьбое2жиое1с2ое2сто2етоо3жди2озавоз2вио1здрозе1ооз3но2озоно2зопоз1уго2зымо3зысои2г1оиг2нои3мо2ок2ло3клюоко1бок1ск2окти2окумом2блом1риом2шео3мьяоно1боо3псоос3мо2отио3пако3паро2плиоп2лоо2пляоп2риоп2тоо1р2вор2тро1руео1руко1русо3садо2скеос1кио1с2лос3миос2пяос2свос2тао2сучо1с2чот3ваот1веот1виот1вло3терот1риот3смоту2ао3тьюо3тьяоус2коу3таоу3то2офаш2офит2офон2офото2фриох1лыо2хляох2ме2охороча1соч1лео3члиош3ваош2лаоэ1тиоя2рипави3пав3лпа2вьпа2дрпа2енпа1зопас1лпа2унпа1хупа2шт2п1в2пе2двпе3запе3зопе2льпе4плпе2снпе2сцпе2счпе2трпе2шт3пинк3пися4пла_пла1с2пленп1лею2плив2пло_2плов2плог2плый2плымп1лынп1лых2плю_п1лютп2лясп2ляшп3но1по1б2по3влпое2лпое2хпо1знпои2щ3полкп1оргпор2жпо1рупо1с43послпо3сспот2впот2рпо1х2поэ3мппо1д3превпре1зпрей2пре1л3претпри3вприг2при3кпри3лприп2п2риц3проипро3п2п1с2п3синп2т3впуг3нпу1стпу3тьпэ1рара2бл1рабора2гвра2глрад2жра2дцрак2в1ралг1рамк1рамн1раслрас3прас1трат1в2рахи1ращи2раятрб2лар2блерб2лорб2люрбо3ср3вакр3варр3вежр2вео1рветр3винр2витр2г1лрда1ср2д1врди2ардос2ре1вррег2нрее2врее2дрее2л1резкре1зррез2у1рейш1рекш3ремо1ренк1реньре1онре1опре1ох1репьре3р2ре1слре1счре1твре1чтре3шлр3жа_р3жамр3жанр3ж2др1з2ври3бр2риги2риджрие2лрие3рриз2врик2р1ринсрио2зрио2сри1отри3р2ри1с2ри3сб2риспри2флри3фрри1хлр2к1л2р1л2рнас4рне3оро2влро1двро1длро1др1родьрое2лрое2мрое2хро1зр1рокрро3псро1руро1ск1рослро1смрос2ф1росш1росю1роткроуг2ро2фрро1хлрош2лро3шн1роязрп2лор2плюрств2р2т1врт3варт2влрт1рарт1рерт1риртус1р3тьюрт1яч1рубаруг3н2руксрус1крус3лру3ть1руха1рухо1ручнр3ш2мры2двры2клры2х1ря1виса2блса2дьса2квса2клс1аппса1трса2унса1х22с3бусег2нсе1з2се1квсек1лсекс4семи1се2сксе2стси1омси1опси2пл2скам2скахск2вас2квис2кляс1кон2скошс1кра2скуе1слав1сладс1ламс3левс3леес1лейслео2с1летс3лею2слицс2ложс1люс2с3ля1смесс4меяс3мурс2нас2сная2сную2с3нысов2рсо1з2со3м2со1русо1сксо2сьсот2рсо1члсош2лс2павс2пеес2пелс2пенс2пехс2пешс2пеюс2пим2сполс2посс2рабсра2сс1ратсс3во4с5сис3с2к1ста_4ств_2ствлст2вя1стей1стелсте3хс3тешс2тиес2тиис2тичс2тиюст2ла2стли2стля1сто_1стов1стог1стод1стое1сток1стом1стон1стос1стотс2тоц1стою2стр_с1тут1стуюс2тыв2сть_2стьс3стью1стья1стям1стяхсуб1осу3глсу2евсу1крсума1супе2сус3лсус3псу1стсут1рсу2ф31с2фе1с2хе2с3цис2часс3чив2счикс2читсъе3дсъе3лсы2г1ся3тьта2блтаб2рта2гнта1з2та2плта1стта1тр2т1б22т2ват1вейт1велт1ветт1воет1вос2твою2т1врте2гнте1зо3текатек1л3текште1ох3терзтер3к3терятест2те2хотиа2мти2блти3д2тиис1т1импт1инд2тинж2тинфти1хр2т1к2тло2бтми2с2тобъто2влто1з2ток2р2томс2тонг1торг1торж1торсто1ру1торш2тотдто3тктпа1ттрдо2т1реат1регт1редт1реет1рецт1рею1трибт1ривт1рилт1римтри1от1риттри3фт1рищ2тройт1рортро3т2трою1трубт2руд2трукт2румт2рутт1ря_т1рявт1ряет1ряжт1ряйт3рякт1рятт1рящт1ряя4т1с2т2с3дтсеп2т2с3мт2с3пту2грту1слту1стту2фл1туша1тушо1тушьты2г12тя2чу2алеу3белубо1дубос21убрауб3рюу1ве_уг2науг2неуг1реуг1ряуда1суд1роуес2лу1з2вузо3пуко1бу1ку1у1лыху2озауост1уо2т1уп1люу3проурке3у2родурт2ру2садус1каус1киуск3лу1скрус3лиу1стеу1стяу3сьяу3терут2ляут1риу1тьеу3тьюуф2ляух1адуха2тух3ляу2чебуш1лафа2б1фа2гнфа1зофан2дфа1трфев1рфед1рфе1о3фи2глфи2зо2фобъфо2рвфо1руфос1кф1рабфра1зфра1сф1ратф2ренфре2сф2рижф2ризф2ронф2торфу3тлха2бл2х1акхан2дх1арш2х3ве2х3вихиат1хи1с2х1лавх1ласх1латх1лац1хлебх2лесх1летх3ло_х2лоп1х2му3х2ныхо2пехо1рух1осмхох1лх1раз1хранх1рейх2рисх1ров1хром2х1с2х1у2гх1у2рху3ра2х1ч2ца2плце1отцеп1лцес2лци2к1цик3лци2олци2скциф1р2ц1к2ц1о2б2ц1от2ц3ш2цып3лча2дрча2дцча2ер3чато3чатыче1влче2глчер2сче1сл1ч2лач3легч3лежч2ли_1ч2ло2ч1таша2блша2гнша2дрша1стш3венше2глше1о2ше3плше1с2ши2блши2плшиф1р2ш1к22шленш2ли_2шлив2шлилш2линш2лисш2лифш2ло_2шловш2лог2шляе2шлякш2ляп2шлят2шляч2шляю3ш2мыш2нуршу2евшуст12щ3в2ще2глщед1рщеис1ще3шкъе3доъ2е2ръе2хиыд2реы2дряы3ж2дыз2ваыз2наы2к1выко1зыре2хыс1киыс1куыт1виы3тьюы3тьяы2ш1лье1зоьми3дьми3кьне2оь2п1ль2стиь2стяьти3мь2тотьт2реьт2руьт2рыьхоз1ь3ягсэк2стэле1оэпи3кэс3теэт1раюзи2кю2к1вюре4мю2с1кю1стаю1стею1стою1стяюха1сяб1раяб3реяб1рияб3рюя1в2хя2г1ляз2гня2к1вя2к1ляст3вя1стояст1ряти1зя3тьюя3тьяа2ньшгст4ре2мьдзаи2лзао2ззаю2лз2рятзу2мьпое2ж2стьт6хуя_ы2рьмыя2вяьбат2а2двё2алёк2амёта1тьёб3лёнб2люёб1рёкб2рёмб2рёх3везёвёд1р2вёрдв2лёкв2лётв2нёс3всё3г2лётг2нёвг3нёнг2ноёд2рёбд2рёмдъё2м2евёре2глёер1вёет1вёе1тьёё1браёб1рыё1друё1зом2ё1ко2ё1крёк2ро2ёмужёпат2ё3плаёп1луё3плыё3ре_ёр3скё3с2аё2скеё3сту2ётечёто1сёха2тёх1ружё1с2з2вёзз2наёз2отёзъё2м2зымё2и1вёих1рёк3лёнк2роёлё3долёк1ллё2ск2лоён1льщё3м2щёнд2рёнё1б23номёоё2жио2скёот1вёо3тёрпё2тр2плёнп1лёюпоё2ж3прётр2блё1рвёт1рёзкрёз2у1рёкш3рёмо1рёнкроё2мсёкс4сё2ст2скуёс1лёт1стёлстё3хс3тёшт1вёлт1воётё2гнтё1зо3тёкатёк1л3тёкштёр3ктё2хоуг2нёуг1рёу1стёу3тёру1тьёу2чёб2х3вё1хлёбх2лёсчёр2с2шлёнъ2ё2рыд2рёырё2хьё1зояб3рё",
        6: "_аг1ро_аль3я_ас1то_аст1р_де1кв_ди2ак_до3т2_зав2р_ио4на_лес1к_люст1_ми1ом_мо2к1_на3ш2_не3вн_не1др_не1з2_не1сл_нос1к_нук1л_ос2ка_ос3пи_от1ро_от1ру_от1уж_по3в2_по3ж2_поз2н_прос2_ре2бр_ри2ск_септ2_те2о3_тиг1р_уз2наабе3ста3в2чеага1с2а2гитиа2глосаг2лотади2ода2д1руаза4ш3аз3веза2зовьа2зольа1зориаз2о1сак3лемако1б22аконсалуш1та2минтам2нетамо1з2ана2дцан2драан2сура2н1узап2ломапо4всап1релара2стар2бокар2валаре1дваре1олар2торар2т1р1ассигаст1вуас3темас2тинас2тияас1тооас1туха1стьеас2шедас2шесат1обеа2томнат1рахба2г1рбе2д1рбез1а2без5д4без1о2бе2с1кбе2с1тбес3тебес3ти1б2лазб3лази1б2лее1б2лея1б2луж2б3лю_бо1брабо1драбо1л2жбо1льсбо3м2лбо3скобо3стибра1зо1б2рал2б1рамб2рать1б2рач2б3рая1б2редб2ритоб2ритыб1ром_3брукс2б3рю_бу2г1рва2д1рва3ж2два2стрве2с1квзъе3д3в2кус2в3лаб3в2нук3в2нучвои2с1вос3пево2стрво3х2т2в1рам2в1рах2в1рен1в2ризвро3т2в3ская4в3ски4в3скувто1б2ву2х1а3в2шиввы3ш2лга1ст2г1лами2глась3г2лифг3лоблгнит2рго3ж2дго2с1аго1склго1спагу2с1кда2гендаст1р2д1вид2двинт2двинч2д1вис2д1вит1дворьде1б2лде1б2рдез1о2дерас2де2с3вди2алиди2алодио3деди1отиди3фто3дневн4д3но1дно3д23д2няшдо3в2мдо3ж2д2долимдо2м1р2допледо2предо2рубдот2ридо2ш3вдо3ш2кдо2шлы1дравш2дразвд1ране2д3реж1дрема1дремлдрем3н1дремы2д3рендре2скд2ресс1д2рож2д3роз1д2рыг1д2рягду2ста2дут1рды2г1р2ды2с1еб1ренеб1рове2б3рюе3в2меев2нимев2нятевра1с2е1вреев1рееев1рейев1реяега1с2е2гланедноу3ед1опре2дотве2д1още2дру_е2ду2бед1убое2дувеед1уст2е3душе2евидее2в1реест1ре4ждевеза2вре1з2ваез1о2гез1о2рез1у2дез1у2кезу2соезу2сыез1у2хез1учаеис1трек1стееле3скеле1сцеми3д2ен2д1реоб2рое2о3глео2гроеоде3зе2о3роеост1реот2руепа1трепис2кеп1лешеп1лющер1актере3доере1дрере1к2ере1х4ерио3зер1обл2ерови2ерокреро3ф2ес1кале2сковес1ласес2линес2ловес2ломес2пекес3полес2танес2четеук2лоефи3б2ех1атоех3валех3лопех1опоех1у2ч3ж2дел4ждемеже1к2вза2вруза3ж2дза3мнеза3р2д2з3ва_з3валь1з2вон2з1вуюзи2онози3т2рзко3п2зо3в2мзо2о3пзот2резот2ризро2с3зу2б3р2з1уз3з1у2моз1у2тезу2час2зы2г12зы2с1иа2зовиа2налиа1с2киа1стаиа1стоиат1роиг1рени2г1ряиди1омиди1оти2еводиз2гнеиз2налика1с2ик2с1тило1ски2менои2мену2имень1инстии3оновио3склио1с2пио2т1випа1трипо3к2ира2сти2р1ауири2скиро1з2ис3ка_ис3камис3кахис3ковис3ку_и2сламисо2ски2с3при2ст1вис1тязи2т1веит2ресит3роми2т1учи2х1асих2ло2ихлор1й2с3мука2брика3днека2д1рка2п1лка2прекар3трка1т2рка2ш1тке2с1кке2ст12к3ла_2к3ле_к3лем_2к3ли_2к3лив2к3лис2к3ло_2к3лос2к3лю_3к2ниж3к2няж1кольс2коминко2р3вкре2слкри2о3ксанд2к1стамк1стан3к2то_ку2п1рла3ж2д1лами_ла1сталаст1вла1стела1стола1стула1стяла1т2р1л2галлев1рале2г1лле1онтле1о2сле4скале1с2лле1спеле1т2рли2гро2л1испли2х3вло1б2р2ловия3ловодло2г3длого1слок3ла3лопас2л1оргло1с2плу1д4р1льсти1льстяма2к1р2м1аллма1с4тма2тобма2т1рме2с1кми2гремик1рими1опими1с2л3м2нешмоис1тмо2к3вмос1камо1с2пмо2т1рм2с1ор3м2стиму1с2кму1с4лнаби1она1в2рна3м2нна1рвана1т2рн1а2фрна3ш2лнд1рагнд1ражнд2риане1в2дне3вняне1д2лне2дране1дроне3ж2дне1з2лне1к2вне3м2н3не1о2не2одане1р2жне3с2нне1с2пне1с2хне1с2чне1т2вне3т2лне1т2р2нинспнист2рнко3п2н2к1ронно3п2но3з2оно1склно2слино1с2пн2сконн2с1окн3с2пентр1ажн2трарнтрас2н2тривн2трокнтр1удн2т1ря2н3ю2роб2левоб2лемобо3м2о2бра_о1браво1брано3в2лоо2в1риов3скоог3ла_ог3ли_ог3ло_од1вое2оди3а2о3димод2литодо3про2досио1драгод1ражод1разод1рако1дралод3ребо1дробод1рово2дымао2дымуо2е1вло3ежеко3ж2дуо1з2ваоз2вено1з2вяоз2глооз2доро2з1обозо1ру2о3кан2о3колол2ган1олимполу3д2о3множоне3ф2он2труоост1ро2пле_оп2литоп3лю_о3плясопо4всопоз2но3п2теора2с3ор2б3л2о3регоре2скор1испор1уксоса3ж2о2с3баос3кароск1воо2ски_о2сковос1койос1комос1коюос1куюос3лейос3логос3лыхос3мосос2нялос2пасо1с2пуос2с3мо3страос2цен2о3тек2о3техо3ткалот1работ1радот1разотра2сот1режот1рекот1речот1решот1родот1роеот1рокот1росот1рочот1ругот1у2чо2форио2ч1топас1тапа1степас1топас1тупа1тропери1опе2с1кпиаст1пи2ж3мпи2к1рп1лем_п1лемсп2ленкп1ле2оплес1к3п2ликпо3в2спод1вопо1звепо1здопо1з2лпо3мнопо3мну3по3п2по2шлопо2шлыпо2шляпре1огпри3д2приль2про1блпрод2лпро3ж2про1з2п1розопрофо23п2сал3п2сих3п2тихпус1кура2б1р1равняра2журра2зийра2зуб1ракизра2к3лра2нохран2сцра2п1лрас3к21растара2такра1т2р1р2вавр3ватарег2ляре2досре3ж2дре1з2лре1зна1ре1зоре1к2лре3мноре1о2рре1о2фре1о2црес1кире1с2пре3старе3сторе1т2рреуч3три3в2нри2глори3г2нри1д2рри3м2нри3м2чри3стври3т2рриэти2рне1с2рно3слро2блюро1б2р1рогол1рогруро3д2зрод2ле1розарро1з2в3розысрои2с31рокон1ролис1ролиц1ромор1ронаж1ронап1роносрооп1рро2плю2р1оргро1р2жро2скиро2скуро1с2п1рот2врот2рир3ствлр2таккр2т1обрт1оргрт2ранру2дар1ружейру1старуст1рр2х1инр1х2лор2х1опры2с1к2с1арк2с1атлса2ф1рсбезо3сбе3с22с3венсе2к1рсере2бсе3стасе3стесест1рс2канд1с2каф3скиноск3ляв2сконас2копс2скриб2с3ла_2с3лая2с3ли_2с3ло_с3лому2с3лос2с3лую2с3лые2с3лый2с3лым1с2наб1с2неж2с3никсно1з2со1б2рсо1л2гсо2риесо1с2п1с2пец2списяспо1з2сре2б1сре3доссанд2с3с2нес2сорист1верст2вол1с4те_1стен_с3тет_с3тете2стимп2стинд2стинф2стинъс2тишкс3т2лест2лилст2литс2то1б3с2тои2сторг2сторж2сторсстрас24страя2стредст1рей2стривст1риз2стрил2стрищст1роаст1родст1рохст2рубст1рушсуб1а2с2ценасы2п1лсыс1ката1вритак3лет1во1з2т1войтеле1отем2б1те2о3дте4п1лте2рактере2оте2скате2скути1знатила2м2т1инвти1с2лти3ствти3ф2р3т2кав3т2кан3т2кеттмист1то2бесто1б2лто3д2р2т1оммто1с2нто1с2пто1с2цт1рага2т1раж1требо1требут1ребьт1ревет1ревшт1резат1резнтреп1л3тре2стрес1кт1рестт1ретут2решь4тринст1роглт1роидтро3плт1росо4т3роц2т1рядту2жинты2с1к1у2бытуд1рамуе2с1кун2д1руро2длус1комус1ку_у3х4вофанд1рфе2с1кфиа2к1фи2нин2ф1оргфор3трфото3п2ф1у2п2х1изы1х2лор2х1о2кхо2пор2х1оснхри2плхро2мч2цетат2ц1о2дча2евоча2евычаст1вча1стеча1стуча1стячерст1ша2г1ршан2кршар3т2ша1тро3ш2кол2ш1лейш2лите4ш3мы_ще1б2лщи2п1лы2д1роы2к3лоынос3лыра2с3ье2с1кь3п2тоь2трабэри4трэро1с2эс2т1рэтил1аю2б1рею2идалюри2ск3явиксям2б3л_вст2р_реа2нбезу2свиз2гнвыб2редос2ня4ж3дик4ж3дичла2б1рлу3с4нни4сь_о2плюсоти4днпти4днреж4ди2стче_сы2мит2сься_аз3вёзам2нётас3тёмбё2д1р2в1рён2доплёдо2прё2д3рёж1дрёма1дрёмы2д3рёнеб1рён2е1врёерё3доерё1к2ес2чёт2ё1вре2ё3душёз1о2гён2д1рёс2танёх1атоёх3валёх3лопёх1опоза3мнёзот2рёиг1рёнла1стёлё4ска3м2нёшод3рёб2о3тёкот1рёкот1рёшп2лёнкплёс1к_рё2бррё1зна1рё1зорё3старё3стород2лёсе3стёсёст1р1стён_с3тёт_с3тётес3т2лётё4п1лтё2скатё2ску3т2кётт1ревёт2рёшьчёрст1",
        7: "_во2б3л_во3ж2д_за3м2н_ле2п3р_му2шт1_не1с2ц_обо3ж2_ра2с3т_ре2з3в_ро2з3в_ро2с3л_хо2р3в_че2с1ка2д1облаз2о1бра2н1о2бан1о2храпо3ч2тбили3т2б2лес1к2б3люсь1б2роди1б2росибро2с1кве2ст1вви2а1с2ви1с2ниво2б3лагри4в3нде2з1а2ди2с1тр2д1обладо1б2рад1о2сенд1о2син2д1осно2д1отря1д2разнд1ра2с3дро2г3неан2д1ре1д2лине1о2свие3п2лодере3м2не2р1у2пе2с1ка_е4с1ку_2ж1о2т1за2в1ри1з2о3ре2з1у2беи2л1а2ци2л1у2пино2к3лино3п2лисан2д1ки1с2ни2к3ласько1б2рикохо2р3ла2д1аглан2д1рла2ст1рле1з2о3лу3п2ломан2д1рме2ж1атме2ст1рна2и1с2на1р2вине2р1отни2л1ални2л1ам2н1инстнти1о2кобо1л2го3в2нуш1о2деяло2д1отчо2д1у2чоза2б3воко3п2ло3м2немо3м2нето2п1лейопо2ш3лоро2с3ло2с1ка_о1с2копо2с1ку_о1с2нимо1с2шивошпа2к3па2с1тыпе2д1инпе2к1лапе2ст1рподо3м2радо1б2рас3т2лрво1з2дремо2г3рес2с3мро2д1отро2ф1акр2т1акт2с1альп2сбе3з2сто2г3нс4т1ровсче2с1кте2с1ките2с1ко3т2ре2хтри2г1л2т1у2пруре2т3русла4ж3уто3п2сх1ра1с2ь2т1амп_бо2дра_об2люю_об2рее_об2рей_об2рею_об2рив_об2рил_об2рит_пом2ну_со2плаатро2скбино2скдро2ж3ж2дружейилло3к2ме2динсмис4с3ннар2ватне2о3рен2трассо4ж3девойс4ково2м3че_он2тратосо4м3нпо2додепо2стинпрем2норедо4плроб2лею2сбрук1б2лёс1кё2с1ка_ё4с1ку_1з2о3рёлё1з2о3о3м2нёмо3м2нёто2п1лёйпё2ст1рсчё2с1ктё2с1китё2с1ко3т2рё2х_чё2с1к",
        8: "_ар2т1о2_ме2ж1у2а2н1а2ме2д1о2бедло2к1а2ун2тр1а2го2д1о2пео2д1о2пыпо2д1о2кре2д1о2пр2т1у2чи_доб2рел_до1б2ри_па2н1ис_ро2с3пиди1с2лове2о3позиере3с2со2з1а2хавни1с2коло1и2с1трони3л2ампере1с2нсо2стритсо3т2калтро2етес_доб2рёлтро2етёс",
        9: "е2о3платои2л1а2минме2д1о2сммети2л1амо2д1о2болпо2д1у2роприче2с1крни3л2а3мпричё2с1к",
        10: "но4л1а2мин"
    }
};

var hyp = function (value) {
    return Hyphenator.hyphenate(value, 'ru');
}