/*
Copyright (c) 2008, Yahoo! Inc. All rights reserved.
Code licensed under the BSD License:
http://developer.yahoo.net/yui/license.txt
version: 2.5.1
*/
YAHOO.util.Color=function(){var A="0123456789ABCDEF",B=YAHOO.lang;return{real2dec:function(C){return Math.min(255,Math.round(C*256));},hsv2rgb:function(G,N,L){if(B.isArray(G)){return this.hsv2rgb.call(this,G[0],G[1],G[2]);}var C,H,K,F,I,E,D,M;F=Math.floor((G/60)%6);I=(G/60)-F;E=L*(1-N);D=L*(1-I*N);M=L*(1-(1-I)*N);switch(F){case 0:C=L;H=M;K=E;break;case 1:C=D;H=L;K=E;break;case 2:C=E;H=L;K=M;break;case 3:C=E;H=D;K=L;break;case 4:C=M;H=E;K=L;break;case 5:C=L;H=E;K=D;break;}var J=this.real2dec;return[J(C),J(H),J(K)];},rgb2hsv:function(C,G,H){if(B.isArray(C)){return this.rgb2hsv.call(this,C[0],C[1],C[2]);}C=C/255;G=G/255;H=H/255;var D,I,K,F,L,J;D=Math.min(Math.min(C,G),H);I=Math.max(Math.max(C,G),H);K=I-D;switch(I){case D:F=0;break;case C:F=60*(G-H)/K;if(G<H){F+=360;}break;case G:F=(60*(H-C)/K)+120;break;case H:F=(60*(C-G)/K)+240;break;}L=(I===0)?0:1-(D/I);var E=[Math.round(F),L,I];return E;},rgb2hex:function(E,D,C){if(B.isArray(E)){return this.rgb2hex.call(this,E[0],E[1],E[2]);}var F=this.dec2hex;return F(E)+F(D)+F(C);},dec2hex:function(C){C=parseInt(C,10);C=(B.isNumber(C))?C:0;C=(C>255||C<0)?0:C;return A.charAt((C-C%16)/16)+A.charAt(C%16);},hex2dec:function(E){var D=function(F){return A.indexOf(F.toUpperCase());};var C=E.split("");return((D(C[0])*16)+D(C[1]));},hex2rgb:function(C){var D=this.hex2dec;return[D(C.substr(0,2)),D(C.substr(2,2)),D(C.substr(4,2))];},websafe:function(E,D,C){if(B.isArray(E)){return this.websafe.call(this,E[0],E[1],E[2]);}var F=function(G){if(B.isNumber(G)){G=Math.min(Math.max(0,G),255);var H,I;for(H=0;H<256;H=H+51){I=H+51;if(G>=H&&G<=I){return(G-H>25)?I:H;}}}return G;};return[F(E),F(D),F(C)];}};}();(function(){var E=0;var R=function(){var b=document.createElement("div");if(this.CSS.BASE){b.className=this.CSS.BASE;}return b;};YAHOO.widget.ColorPicker=function(h,b){E=E+1;b=b||{};if(arguments.length===1&&!YAHOO.lang.isString(h)&&!h.nodeName){b=h;h=b.element||null;}if(!h&&!b.element){h=R.call(this,b);}YAHOO.widget.ColorPicker.superclass.constructor.call(this,h,b);};YAHOO.extend(YAHOO.widget.ColorPicker,YAHOO.util.Element);var Q=YAHOO.widget.ColorPicker.prototype,P=YAHOO.widget.Slider,e=YAHOO.util.Color,C=YAHOO.util.Dom,f=YAHOO.util.Event,g=YAHOO.lang,J=g.substitute;var a="yui-picker";Q.ID={R:a+"-r",R_HEX:a+"-rhex",G:a+"-g",G_HEX:a+"-ghex",B:a+"-b",B_HEX:a+"-bhex",H:a+"-h",S:a+"-s",V:a+"-v",PICKER_BG:a+"-bg",PICKER_THUMB:a+"-thumb",HUE_BG:a+"-hue-bg",HUE_THUMB:a+"-hue-thumb",HEX:a+"-hex",SWATCH:a+"-swatch",WEBSAFE_SWATCH:a+"-websafe-swatch",CONTROLS:a+"-controls",RGB_CONTROLS:a+"-rgb-controls",HSV_CONTROLS:a+"-hsv-controls",HEX_CONTROLS:a+"-hex-controls",HEX_SUMMARY:a+"-hex-summary",CONTROLS_LABEL:a+"-controls-label"};Q.TXT={ILLEGAL_HEX:"Illegal hex value entered",SHOW_CONTROLS:"Show color details",HIDE_CONTROLS:"Hide color details",CURRENT_COLOR:"Currently selected color: {rgb}",CLOSEST_WEBSAFE:"Closest websafe color: {rgb}. Click to select.",R:"R",G:"G",B:"B",H:"H",S:"S",V:"V",HEX:"#",DEG:"\u00B0",PERCENT:"%"};Q.IMAGE={PICKER_THUMB:"../../build/colorpicker/assets/picker_thumb.png",HUE_THUMB:"../../build/colorpicker/assets/hue_thumb.png"};Q.DEFAULT={PICKER_SIZE:180};Q.OPT={HUE:"hue",SATURATION:"saturation",VALUE:"value",RED:"red",GREEN:"green",BLUE:"blue",HSV:"hsv",RGB:"rgb",WEBSAFE:"websafe",HEX:"hex",PICKER_SIZE:"pickersize",SHOW_CONTROLS:"showcontrols",SHOW_RGB_CONTROLS:"showrgbcontrols",SHOW_HSV_CONTROLS:"showhsvcontrols",SHOW_HEX_CONTROLS:"showhexcontrols",SHOW_HEX_SUMMARY:"showhexsummary",SHOW_WEBSAFE:"showwebsafe",CONTAINER:"container",IDS:"ids",ELEMENTS:"elements",TXT:"txt",IMAGES:"images",ANIMATE:"animate"};var S=function(){var b=this.get(this.OPT.PICKER_SIZE),i=this.get(this.OPT.HUE);i=b-Math.round(i/360*b);if(i===b){i=0;}this.hueSlider.setValue(i);};var d=function(){var h=this.get(this.OPT.PICKER_SIZE),i=this.get(this.OPT.SATURATION),b=this.get(this.OPT.VALUE);i=Math.round(i*h/100);b=Math.round(h-(b*h/100));this.pickerSlider.setRegionValue(i,b);};var T=function(){S.call(this);d.call(this);};Q.setValue=function(h,b){b=(b)||false;this.set(this.OPT.RGB,h,b);T.call(this);};Q.hueSlider=null;Q.pickerSlider=null;var X=function(){var b=this.get(this.OPT.PICKER_SIZE),i=(b-this.hueSlider.getValue())/b;i=Math.round(i*360);return(i===360)?0:i;};var O=function(){return this.pickerSlider.getXValue()/this.get(this.OPT.PICKER_SIZE);};var N=function(){var b=this.get(this.OPT.PICKER_SIZE);return(b-this.pickerSlider.getYValue())/b;};var M=function(){var i=this.get(this.OPT.RGB),k=this.get(this.OPT.WEBSAFE),j=this.getElement(this.ID.SWATCH),h=i.join(","),b=this.get(this.OPT.TXT);C.setStyle(j,"background-color","rgb("+h+")");j.title=g.substitute(b.CURRENT_COLOR,{"rgb":"#"+this.get(this.OPT.HEX)});j=this.getElement(this.ID.WEBSAFE_SWATCH);h=k.join(",");C.setStyle(j,"background-color","rgb("+h+")");j.title=g.substitute(b.CLOSEST_WEBSAFE,{"rgb":"#"+e.rgb2hex(k)});};var Z=function(){var k=X.call(this),j=O.call(this),b=N.call(this);var i=e.hsv2rgb(k,j,b);this.set(this.OPT.RGB,i);};var B=function(){this.getElement(this.ID.H).value=this.get(this.OPT.HUE);this.getElement(this.ID.S).value=this.get(this.OPT.SATURATION);this.getElement(this.ID.V).value=this.get(this.OPT.VALUE);this.getElement(this.ID.R).value=this.get(this.OPT.RED);this.getElement(this.ID.R_HEX).innerHTML=e.dec2hex(this.get(this.OPT.RED));this.getElement(this.ID.G).value=this.get(this.OPT.GREEN);this.getElement(this.ID.G_HEX).innerHTML=e.dec2hex(this.get(this.OPT.GREEN));this.getElement(this.ID.B).value=this.get(this.OPT.BLUE);this.getElement(this.ID.B_HEX).innerHTML=e.dec2hex(this.get(this.OPT.BLUE));this.getElement(this.ID.HEX).value=this.get(this.OPT.HEX);};var Y=function(k){var i=X.call(this);this.set(this.OPT.HUE,i,true);var b=e.hsv2rgb(i,1,1);var j="rgb("+b.join(",")+")";C.setStyle(this.getElement(this.ID.PICKER_BG),"background-color",j);if(this.hueSlider.valueChangeSource===this.hueSlider.SOURCE_UI_EVENT){Z.call(this);}B.call(this);M.call(this);};var H=function(i){var h=O.call(this),b=N.call(this);this.set(this.OPT.SATURATION,Math.round(h*100),true);
this.set(this.OPT.VALUE,Math.round(b*100),true);if(this.pickerSlider.valueChangeSource===this.pickerSlider.SOURCE_UI_EVENT){Z.call(this);}B.call(this);M.call(this);};var W=function(b){var h=f.getCharCode(b);if(h===38){return 3;}else{if(h===13){return 6;}else{if(h===40){return 4;}else{if(h>=48&&h<=57){return 1;}else{if(h>=97&&h<=102){return 2;}else{if(h>=65&&h<=70){return 2;}else{if("8, 9, 13, 27, 37, 39".indexOf(h)>-1){return 5;}else{return 0;}}}}}}}};var I=function(h,b,j){var i=b.value;if(j!==this.OPT.HEX){i=parseInt(i,10);}if(i!==this.get(j)){this.set(j,i);}};var G=function(i,b,k){var j=W(i);var h=(i.shiftKey)?10:1;switch(j){case 6:I.apply(this,arguments);break;case 3:this.set(k,Math.min(this.get(k)+h,255));B.call(this);break;case 4:this.set(k,Math.max(this.get(k)-h,0));B.call(this);break;default:}};var A=function(h,b,j){var i=W(h);if(i===6){I.apply(this,arguments);}};var L=function(h,b){var i=W(h);switch(i){case 6:case 5:case 1:break;case 2:if(b!==true){break;}default:f.stopEvent(h);return false;}};var K=function(b){return L(b,true);};Q.getElement=function(b){return this.get(this.OPT.ELEMENTS)[this.get(this.OPT.IDS)[b]];};var D=function(){var k,j,n,l,m,b=this.get(this.OPT.IDS),o=this.get(this.OPT.TXT),r=this.get(this.OPT.IMAGES),q=function(i,p){var t=document.createElement(i);if(p){g.augmentObject(t,p,true);}return t;},s=function(i,p){var t=g.merge({autocomplete:"off",value:"0",size:3,maxlength:3},p);t.name=t.id;return new q(i,t);};var h=this.get("element");k=new q("div",{id:b[this.ID.PICKER_BG],className:"yui-picker-bg",tabIndex:-1,hideFocus:true});j=new q("div",{id:b[this.ID.PICKER_THUMB],className:"yui-picker-thumb"});n=new q("img",{src:r.PICKER_THUMB});j.appendChild(n);k.appendChild(j);h.appendChild(k);k=new q("div",{id:b[this.ID.HUE_BG],className:"yui-picker-hue-bg",tabIndex:-1,hideFocus:true});j=new q("div",{id:b[this.ID.HUE_THUMB],className:"yui-picker-hue-thumb"});n=new q("img",{src:r.HUE_THUMB});j.appendChild(n);k.appendChild(j);h.appendChild(k);k=new q("div",{id:b[this.ID.CONTROLS],className:"yui-picker-controls"});h.appendChild(k);h=k;k=new q("div",{className:"hd"});j=new q("a",{id:b[this.ID.CONTROLS_LABEL],href:"#"});k.appendChild(j);h.appendChild(k);k=new q("div",{className:"bd"});h.appendChild(k);h=k;k=new q("ul",{id:b[this.ID.RGB_CONTROLS],className:"yui-picker-rgb-controls"});j=new q("li");j.appendChild(document.createTextNode(o.R+" "));l=new s("input",{id:b[this.ID.R],className:"yui-picker-r"});j.appendChild(l);k.appendChild(j);j=new q("li");j.appendChild(document.createTextNode(o.G+" "));l=new s("input",{id:b[this.ID.G],className:"yui-picker-g"});j.appendChild(l);k.appendChild(j);j=new q("li");j.appendChild(document.createTextNode(o.B+" "));l=new s("input",{id:b[this.ID.B],className:"yui-picker-b"});j.appendChild(l);k.appendChild(j);h.appendChild(k);k=new q("ul",{id:b[this.ID.HSV_CONTROLS],className:"yui-picker-hsv-controls"});j=new q("li");j.appendChild(document.createTextNode(o.H+" "));l=new s("input",{id:b[this.ID.H],className:"yui-picker-h"});j.appendChild(l);j.appendChild(document.createTextNode(" "+o.DEG));k.appendChild(j);j=new q("li");j.appendChild(document.createTextNode(o.S+" "));l=new s("input",{id:b[this.ID.S],className:"yui-picker-s"});j.appendChild(l);j.appendChild(document.createTextNode(" "+o.PERCENT));k.appendChild(j);j=new q("li");j.appendChild(document.createTextNode(o.V+" "));l=new s("input",{id:b[this.ID.V],className:"yui-picker-v"});j.appendChild(l);j.appendChild(document.createTextNode(" "+o.PERCENT));k.appendChild(j);h.appendChild(k);k=new q("ul",{id:b[this.ID.HEX_SUMMARY],className:"yui-picker-hex_summary"});j=new q("li",{id:b[this.ID.R_HEX]});k.appendChild(j);j=new q("li",{id:b[this.ID.G_HEX]});k.appendChild(j);j=new q("li",{id:b[this.ID.B_HEX]});k.appendChild(j);h.appendChild(k);k=new q("div",{id:b[this.ID.HEX_CONTROLS],className:"yui-picker-hex-controls"});k.appendChild(document.createTextNode(o.HEX+" "));j=new s("input",{id:b[this.ID.HEX],className:"yui-picker-hex",size:6,maxlength:6});k.appendChild(j);h.appendChild(k);h=this.get("element");k=new q("div",{id:b[this.ID.SWATCH],className:"yui-picker-swatch"});h.appendChild(k);k=new q("div",{id:b[this.ID.WEBSAFE_SWATCH],className:"yui-picker-websafe-swatch"});h.appendChild(k);};var c=function(h,b){f.on(this.getElement(h),"keydown",function(j,i){G.call(i,j,this,b);},this);f.on(this.getElement(h),"keypress",K,this);f.on(this.getElement(h),"blur",function(j,i){I.call(i,j,this,b);},this);};var F=function(){var b=[this.get(this.OPT.RED),this.get(this.OPT.GREEN),this.get(this.OPT.BLUE)];this.set(this.OPT.RGB,b);T.call(this);};Q.initPicker=function(){var m=this.OPT,l=this.get(m.IDS),h=this.get(m.ELEMENTS),b,k,n;for(b in this.ID){if(g.hasOwnProperty(this.ID,b)){l[this.ID[b]]=l[b];}}k=C.get(l[this.ID.PICKER_BG]);if(!k){D.call(this);}else{}for(b in l){if(g.hasOwnProperty(l,b)){k=C.get(l[b]);n=C.generateId(k);l[b]=n;l[l[b]]=n;h[n]=k;}}h=[m.SHOW_CONTROLS,m.SHOW_RGB_CONTROLS,m.SHOW_HSV_CONTROLS,m.SHOW_HEX_CONTROLS,m.SHOW_HEX_SUMMARY,m.SHOW_WEBSAFE];for(b=0;b<h.length;b=b+1){this.set(h[b],this.get(h[b]));}var j=this.get(m.PICKER_SIZE);this.hueSlider=P.getVertSlider(this.getElement(this.ID.HUE_BG),this.getElement(this.ID.HUE_THUMB),0,j);this.hueSlider.subscribe("change",Y,this,true);this.pickerSlider=P.getSliderRegion(this.getElement(this.ID.PICKER_BG),this.getElement(this.ID.PICKER_THUMB),0,j,0,j);this.pickerSlider.subscribe("change",H,this,true);this.set(m.ANIMATE,this.get(m.ANIMATE));f.on(this.getElement(this.ID.WEBSAFE_SWATCH),"click",function(i){this.setValue(this.get(m.WEBSAFE));},this,true);f.on(this.getElement(this.ID.CONTROLS_LABEL),"click",function(i){this.set(m.SHOW_CONTROLS,!this.get(m.SHOW_CONTROLS));f.preventDefault(i);},this,true);c.call(this,this.ID.R,this.OPT.RED);c.call(this,this.ID.G,this.OPT.GREEN);c.call(this,this.ID.B,this.OPT.BLUE);c.call(this,this.ID.H,this.OPT.HUE);c.call(this,this.ID.S,this.OPT.SATURATION);c.call(this,this.ID.V,this.OPT.VALUE);f.on(this.getElement(this.ID.HEX),"keydown",function(o,i){A.call(i,o,this,i.OPT.HEX);
},this);f.on(this.getElement(this.ID.HEX),"keypress",L,this);f.on(this.getElement(this.ID.HEX),"blur",function(o,i){I.call(i,o,this,i.OPT.HEX);},this);F.call(this);};var U=function(){var h=[this.get(this.OPT.HUE),this.get(this.OPT.SATURATION)/100,this.get(this.OPT.VALUE)/100];var b=e.hsv2rgb(h);this.set(this.OPT.RGB,b);T.call(this);};var V=function(){var k=this.get(this.OPT.HEX),b=k.length;if(b===3){var m=k.split(""),j;for(j=0;j<b;j=j+1){m[j]=m[j]+m[j];}k=m.join("");}if(k.length!==6){return false;}var h=e.hex2rgb(k);this.setValue(h);};Q.initAttributes=function(b){b=b||{};YAHOO.widget.ColorPicker.superclass.initAttributes.call(this,b);this.setAttributeConfig(this.OPT.PICKER_SIZE,{value:b.size||this.DEFAULT.PICKER_SIZE});this.setAttributeConfig(this.OPT.HUE,{value:b.hue||0,validator:g.isNumber});this.setAttributeConfig(this.OPT.SATURATION,{value:b.saturation||0,validator:g.isNumber});this.setAttributeConfig(this.OPT.VALUE,{value:g.isNumber(b.value)?b.value:100,validator:g.isNumber});this.setAttributeConfig(this.OPT.RED,{value:g.isNumber(b.red)?b.red:255,validator:g.isNumber});this.setAttributeConfig(this.OPT.GREEN,{value:g.isNumber(b.green)?b.green:255,validator:g.isNumber});this.setAttributeConfig(this.OPT.BLUE,{value:g.isNumber(b.blue)?b.blue:255,validator:g.isNumber});this.setAttributeConfig(this.OPT.HEX,{value:b.hex||"FFFFFF",validator:g.isString});this.setAttributeConfig(this.OPT.RGB,{value:b.rgb||[255,255,255],method:function(l){this.set(this.OPT.RED,l[0],true);this.set(this.OPT.GREEN,l[1],true);this.set(this.OPT.BLUE,l[2],true);var n=e.websafe(l);this.set(this.OPT.WEBSAFE,n,true);var m=e.rgb2hex(l);this.set(this.OPT.HEX,m,true);var i=e.rgb2hsv(l);this.set(this.OPT.HUE,i[0],true);this.set(this.OPT.SATURATION,Math.round(i[1]*100),true);this.set(this.OPT.VALUE,Math.round(i[2]*100),true);},readonly:true});this.setAttributeConfig(this.OPT.CONTAINER,{value:null,method:function(i){if(i){i.showEvent.subscribe(function(){this.pickerSlider.focus();},this,true);}}});this.setAttributeConfig(this.OPT.WEBSAFE,{value:b.websafe||[255,255,255]});var j=b.ids||g.merge({},this.ID);if(!b.ids&&E>1){for(var h in j){if(g.hasOwnProperty(j,h)){j[h]=j[h]+E;}}}this.setAttributeConfig(this.OPT.IDS,{value:j,writeonce:true});this.setAttributeConfig(this.OPT.TXT,{value:b.txt||this.TXT,writeonce:true});this.setAttributeConfig(this.OPT.IMAGES,{value:b.images||this.IMAGE,writeonce:true});this.setAttributeConfig(this.OPT.ELEMENTS,{value:{},readonly:true});var k=function(m,i){var l=(g.isString(m)?this.getElement(m):m);C.setStyle(l,"display",(i)?"":"none");};this.setAttributeConfig(this.OPT.SHOW_CONTROLS,{value:g.isBoolean(b.showcontrols)?b.showcontrols:true,method:function(i){var l=C.getElementsByClassName("bd","div",this.getElement(this.ID.CONTROLS))[0];k.call(this,l,i);this.getElement(this.ID.CONTROLS_LABEL).innerHTML=(i)?this.get(this.OPT.TXT).HIDE_CONTROLS:this.get(this.OPT.TXT).SHOW_CONTROLS;}});this.setAttributeConfig(this.OPT.SHOW_RGB_CONTROLS,{value:g.isBoolean(b.showrgbcontrols)?b.showrgbcontrols:true,method:function(i){k.call(this,this.ID.RGB_CONTROLS,i);}});this.setAttributeConfig(this.OPT.SHOW_HSV_CONTROLS,{value:g.isBoolean(b.showhsvcontrols)?b.showhsvcontrols:false,method:function(i){k.call(this,this.ID.HSV_CONTROLS,i);if(i&&this.get(this.OPT.SHOW_HEX_SUMMARY)){this.set(this.OPT.SHOW_HEX_SUMMARY,false);}}});this.setAttributeConfig(this.OPT.SHOW_HEX_CONTROLS,{value:g.isBoolean(b.showhexcontrols)?b.showhexcontrols:false,method:function(i){k.call(this,this.ID.HEX_CONTROLS,i);}});this.setAttributeConfig(this.OPT.SHOW_WEBSAFE,{value:g.isBoolean(b.showwebsafe)?b.showwebsafe:true,method:function(i){k.call(this,this.ID.WEBSAFE_SWATCH,i);}});this.setAttributeConfig(this.OPT.SHOW_HEX_SUMMARY,{value:g.isBoolean(b.showhexsummary)?b.showhexsummary:true,method:function(i){k.call(this,this.ID.HEX_SUMMARY,i);if(i&&this.get(this.OPT.SHOW_HSV_CONTROLS)){this.set(this.OPT.SHOW_HSV_CONTROLS,false);}}});this.setAttributeConfig(this.OPT.ANIMATE,{value:g.isBoolean(b.animate)?b.animate:true,method:function(i){this.pickerSlider.animate=i;this.hueSlider.animate=i;}});this.on(this.OPT.HUE+"Change",U,this,true);this.on(this.OPT.SATURATION+"Change",U,this,true);this.on(this.OPT.VALUE+"Change",d,this,true);this.on(this.OPT.RED+"Change",F,this,true);this.on(this.OPT.GREEN+"Change",F,this,true);this.on(this.OPT.BLUE+"Change",F,this,true);this.on(this.OPT.HEX+"Change",V,this,true);this.initPicker();};})();YAHOO.register("colorpicker",YAHOO.widget.ColorPicker,{version:"2.5.1",build:"984"});