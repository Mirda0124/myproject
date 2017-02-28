
TINY={};

TINY.box= new function(a){
	var j,m,b,g,v,p=0;
  
	return{
		show:function(o){
			v={opacity:70,close:1,animate:1,fixed:1,mask:1,maskid:'',boxid:'',top:'250px',left:'center',topsplit:2,url:0,post:0,height:0,width:0,html:0,i:0,close_butt:0,id:0};
			for(s in o){v[s]=o[s]}                
			if(!p){
				j=document.createElement('div'); j.className='tbox';
				p=document.createElement('div'); p.className='tinner';
				b=document.createElement('div'); b.className='tcontent';
				m=document.createElement('div'); m.className='tmask';
		    g=document.createElement('div'); g.className='tclose'; g.v=0;
				document.body.appendChild(m); document.body.appendChild(j); j.appendChild(p); p.appendChild(b);
	   		m.onclick=g.onclick=TINY.box.hide; window.onresize=TINY.box.resize
        }else{
				j.style.display='none'; clearTimeout(p.ah); 	if(v.close_butt==1){if(g.v){p.removeChild(g); g.v=0}}
			    }    
			p.id=v.boxid; m.id=v.maskid; j.style.position=v.fixed?'fixed':'absolute';
   if (v.fixed==false){   
 
                   j.style.top=v.top; 
                   j.style.left=v.left;
                   }
      
      
			if(v.html&&!v.animate){
				p.style.backgroundImage='none'; b.innerHTML=v.html; b.style.display='';
				p.style.width=v.width?v.width+'px':'auto'; p.style.height=v.height?v.height+'px':'auto'
			}else{
				b.style.display='none'; 
				if(!v.animate&&v.width&&v.height){
					p.style.width=v.width+'px'; p.style.height=v.height+'px'
				}else{
					p.style.width=p.style.height='50px'
				}
			}
			if(v.mask){this.mask(); this.alpha(m,1,v.opacity)}else{this.alpha(j,1,100)}
			if(v.autohide){p.ah=setTimeout(TINY.box.hide,1000*v.autohide)}  else 	{document.onkeyup=TINY.box.esc }
		},
		fill:function(c,u,k,a,w,h){
			if(u){
				if(v.image){
					var i=new Image(); i.onload=function(){w=w||i.width; h=h||i.height; TINY.box.psh(i,a,w,h)}; i.src=v.image
				}else if(v.iframe){
					this.psh('<iframe src="'+v.iframe+'" width="'+v.width+'" frameborder="0" height="'+v.height+'"></iframe>',a,w,h)
				}else{
					var x=window.XMLHttpRequest?new XMLHttpRequest():new ActiveXObject('Microsoft.XMLHTTP');
					x.onreadystatechange=function(){
						if(x.readyState==4&&x.status==200){p.style.backgroundImage=''; TINY.box.psh(x.responseText,a,w,h)}
					};
					if(k){
    	            	x.open('POST',c,true); x.setRequestHeader('Content-type','application/x-www-form-urlencoded;'); x.send(k)
					}else{
       	         		x.open('GET',c,true); x.send(null)
					}
				}
			}else{
				this.psh(c,a,w,h)
			}
		},
		psh:function(c,a,w,h){
			if(typeof c=='object'){b.appendChild(c)}else{b.innerHTML=c}
			var x=p.style.width, y=p.style.height;
			if(!w||!h){
				p.style.width=w?w+'px':''; p.style.height=h?h+'px':''; b.style.display='';
				if(!h){h=parseInt(b.offsetHeight)}
				if(!w){w=parseInt(b.offsetWidth)}
				b.style.display='none'
			}
			p.style.width=x; p.style.height=y;
			this.size(w,h,a)
		},

		hide:function(){TINY.box.alpha(j,-1,0,3); document.onkeypress=null; if(v.closejs){v.closejs()}},


		pos:function(){
			var t;
			if(typeof v.top!='undefined'){t=v.top}else{t=(this.height()/v.topsplit)-(j.offsetHeight/2); t=t<20?20:t}
			if(!v.fixed&&!v.top){t+=this.top()}
			j.style.top=t+'px'; 
			j.style.left=typeof v.left!='undefined'?v.left+'px':(this.width()/2)-(j.offsetWidth/2)+'px'
		},
		alpha:function(e,d,a){
			clearInterval(e.ai);
			if(d){e.style.opacity=0; e.style.filter='alpha(opacity=0)'; e.style.display='block'; TINY.box.pos()}
			e.ai=setInterval(function(){TINY.box.ta(e,a,d)},20)
		},
		ta:function(e,a,d){
			var o=Math.round(e.style.opacity*100);
			if(o==a){
				clearInterval(e.ai);
				if(d==-1){
					e.style.display='none';
					e==j?TINY.box.alpha(m,-1,0,2):b.innerHTML=p.style.backgroundImage=''
				}else{
					if(e==m){
						this.alpha(j,1,100)
					}
     else{
						j.style.filter='';
				if (v.loading==0)	{	TINY.box.fill(v.html||v.url,v.url||v.iframe||v.image,v.post,v.animate,v.width,v.height)
					}       }
				}
			}else{
				var n=a-Math.floor(Math.abs(a-o)*.5)*d;
				e.style.opacity=n/100; e.style.filter='alpha(opacity='+n+')'
			}
		},
		size:function(w,h,a){
			if(a){
				clearInterval(p.si); var wd=parseInt(p.style.width)>w?-1:1, hd=parseInt(p.style.height)>h?-1:1;
				p.si=setInterval(function(){TINY.box.ts(w,wd,h,hd)},20)
			}else{
				p.style.backgroundImage='none'; if(v.close){p.appendChild(g); g.v=1}
				p.style.width=w+'px'; p.style.height=h+'px'; b.style.display=''; this.pos();
				if(v.openjs){v.openjs()}
			}
		},
		ts:function(w,wd,h,hd){
			var cw=parseInt(p.style.width), ch=parseInt(p.style.height);
			if(cw==w&&ch==h){
				clearInterval(p.si); p.style.backgroundImage='none'; b.style.display='block'; 	if(v.close_butt==1){if(v.close){p.appendChild(g); g.v=1} }
				if(v.openjs){v.openjs()}
			}else{
				if(cw!=w){p.style.width=(w-Math.floor(Math.abs(w-cw)*.6)*wd)+'px'}
				if(ch!=h){p.style.height=(h-Math.floor(Math.abs(h-ch)*.6)*hd)+'px'}
				this.pos()
			}
		},


	}
}();






  



                                    
TINY1={};

TINY1.box= new function(a){
	var j,m,b,g,v,p=0;
  
	return{
		show:function(o){
			v={opacity:70,close:1,animate:1,fixed:1,mask:1,maskid:'',boxid:'',top:'250px',left:'center',topsplit:2,url:0,post:0,height:0,width:0,html:0,i:0,close_butt:0,id:0};
			for(s in o){v[s]=o[s]}                
			if(!p){
				j=document.createElement('div'); j.className='tbox';
				p=document.createElement('div'); p.className='tinner';
				b=document.createElement('div'); b.className='tcontent';
				m=document.createElement('div'); m.className='tmask';
		    g=document.createElement('div'); g.className='tclose'; g.v=0;
				document.body.appendChild(m); document.body.appendChild(j); j.appendChild(p); p.appendChild(b);
        m.onclick=g.onclick=TINY1.box.hide; window.onresize=TINY1.box.resize
        }else{
				j.style.display='none'; clearTimeout(p.ah); 	if(v.close_butt==1){if(g.v){p.removeChild(g); g.v=0}}
			    }    
			p.id=v.boxid; m.id=v.maskid; j.style.position=v.fixed?'fixed':'absolute';
   if (v.fixed==false){   
 
                   j.style.top=v.top; 
                   j.style.left=v.left;
                   }
      
      
			if(v.html&&!v.animate){
				p.style.backgroundImage='none'; b.innerHTML=v.html; b.style.display='';
				p.style.width=v.width?v.width+'px':'auto'; p.style.height=v.height?v.height+'px':'auto'
			}else{
				b.style.display='none'; 
				if(!v.animate&&v.width&&v.height){
					p.style.width=v.width+'px'; p.style.height=v.height+'px'
				}else{
					p.style.width=p.style.height='50px'
				}
			}
			if(v.mask){this.mask(); this.alpha(m,1,v.opacity)}else{this.alpha(j,1,100)}
			if(v.autohide){p.ah=setTimeout(TINY1.box.hide,1000*v.autohide)}  else 	{document.onkeyup=TINY1.box.esc }
		},
		fill:function(c,u,k,a,w,h){
			if(u){
				if(v.image){
					var i=new Image(); i.onload=function(){w=w||i.width; h=h||i.height; TINY1.box.psh(i,a,w,h)}; i.src=v.image
				}else if(v.iframe){
					this.psh('<iframe src="'+v.iframe+'" width="'+v.width+'" frameborder="0" height="'+v.height+'"></iframe>',a,w,h)
				}else{
					var x=window.XMLHttpRequest?new XMLHttpRequest():new ActiveXObject('Microsoft.XMLHTTP');
					x.onreadystatechange=function(){
						if(x.readyState==4&&x.status==200){p.style.backgroundImage=''; TINY1.box.psh(x.responseText,a,w,h)}
					};
					if(k){
    	            	x.open('POST',c,true); x.setRequestHeader('Content-type','application/x-www-form-urlencoded;'); x.send(k)
					}else{
       	         		x.open('GET',c,true); x.send(null)
					}
				}
			}else{
				this.psh(c,a,w,h)
			}
		},
		psh:function(c,a,w,h){
			if(typeof c=='object'){b.appendChild(c)}else{b.innerHTML=c}
			var x=p.style.width, y=p.style.height;
			if(!w||!h){
				p.style.width=w?w+'px':''; p.style.height=h?h+'px':''; b.style.display='';
				if(!h){h=parseInt(b.offsetHeight)}
				if(!w){w=parseInt(b.offsetWidth)}
				b.style.display='none'
			}
			p.style.width=x; p.style.height=y;
			this.size(w,h,a)
		},

		hide:function(){TINY1.box.alpha(j,-1,0,3); document.onkeypress=null; if(v.closejs){v.closejs()}},


		pos:function(){
			var t;
			if(typeof v.top!='undefined'){t=v.top}else{t=(this.height()/v.topsplit)-(j.offsetHeight/2); t=t<20?20:t}
			if(!v.fixed&&!v.top){t+=this.top()}
			j.style.top=t+'px'; 
			j.style.left=typeof v.left!='undefined'?v.left+'px':(this.width()/2)-(j.offsetWidth/2)+'px'
		},
		alpha:function(e,d,a){
			clearInterval(e.ai);
			if(d){e.style.opacity=0; e.style.filter='alpha(opacity=0)'; e.style.display='block'; TINY1.box.pos()}
			e.ai=setInterval(function(){TINY1.box.ta(e,a,d)},20)
		},
		ta:function(e,a,d){
			var o=Math.round(e.style.opacity*100);
			if(o==a){
				clearInterval(e.ai);
				if(d==-1){
					e.style.display='none';
					e==j?TINY1.box.alpha(m,-1,0,2):b.innerHTML=p.style.backgroundImage=''
				}else{
					if(e==m){
						this.alpha(j,1,100)
					}
     else{
						j.style.filter='';
				if (v.loading==0)	{	TINY1.box.fill(v.html||v.url,v.url||v.iframe||v.image,v.post,v.animate,v.width,v.height)
					}       }
				}
			}else{
				var n=a-Math.floor(Math.abs(a-o)*.5)*d;
				e.style.opacity=n/100; e.style.filter='alpha(opacity='+n+')'
			}
		},
		size:function(w,h,a){
			if(a){
				clearInterval(p.si); var wd=parseInt(p.style.width)>w?-1:1, hd=parseInt(p.style.height)>h?-1:1;
				p.si=setInterval(function(){TINY1.box.ts(w,wd,h,hd)},20)
			}else{
				p.style.backgroundImage='none'; if(v.close){p.appendChild(g); g.v=1}
				p.style.width=w+'px'; p.style.height=h+'px'; b.style.display=''; this.pos();
				if(v.openjs){v.openjs()}
			}
		},
		ts:function(w,wd,h,hd){
			var cw=parseInt(p.style.width), ch=parseInt(p.style.height);
			if(cw==w&&ch==h){
				clearInterval(p.si); p.style.backgroundImage='none'; b.style.display='block'; 	if(v.close_butt==1){if(v.close){p.appendChild(g); g.v=1} }
				if(v.openjs){v.openjs()}
			}else{
				if(cw!=w){p.style.width=(w-Math.floor(Math.abs(w-cw)*.6)*wd)+'px'}
				if(ch!=h){p.style.height=(h-Math.floor(Math.abs(h-ch)*.6)*hd)+'px'}
				this.pos()
			}
		},


	}
}();





//WEB SOOOOOOOOOOOOOOOOOCKET
 
 // 'use strict';
  var connection=null;
  var pripojeno=null;   
  function start(){                
    connection = new WebSocket('ws://'+location.hostname+':81/');
    connection.onopen = function(){    
      console.log('connected!');
      pripojeno=1;
    };
    connection.onmessage = function(e){ console.log(e.data);}
    connection.onclose = function(){
      pripojeno=0;
      console.log('closed!');
      //reconnect now
      check();
    };

    
 
   }
   
   
  //   connection.onclose = function(){ console.log('closed!');  dialog_load=new Messi(  '<IMG SRC="images/loading.gif">',{width:70,modal:true} );}; 
   
 //    setInterval(check, 5000);
    
//     function check(){
     
    
     
//    if(!connection || connection.readyState == 3  ) { console.log('closed!');}
//    console.log('closed');
//  }

  start();
      
 function close(){dialog.unload();}

  var butt=0;
  var msg="";
  var VizCerp=1;
  var dialog_load;
  var PriradCid1;
  var PriradCid2;
  var PriradCid3;
  var PriradCid4;
  var PriradCid5;
  var PriradCid6;
  var PocetCidel; 
  var TimeLast;
  var Time;
  var ztraceno=false;

  
  function odpojeno(){
  if (TimeLast!=Time){TimeLast=Time;dialog_load1.unload}
   if (ztraceno==false){
    if (TimeLast==Time){console.log('closed');var dialog_load1=new Messi('<b>Spojení ztraceno... Pro obnovení zkontrolujte vaše internetové pøipojení a poté znovu naètìte stránku.</b>',{width:743,modal:true,closeButton:false});ztraceno=true;   
                      }
                      }
                      } 
       function spust(){                                    
        setInterval(odpojeno, 5000);
        }
        
    console.log(TimeLast);
    console.log(Time);
    
                           
 connection.onmessage = function(e){
     var server_message = e.data;
   
     var p1 = server_message.search("#");
     var web = server_message.slice(0,p1);
     if (web=="Nastaveni"){  
     p1=p1+1; 
     var msg = server_message.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     document.getElementById('statDiffOn').innerHTML=web;
     p1=p1+1;
     var msg =  msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     document.getElementById('statDiffOff').innerHTML=web;
     p1=p1+1;
     var msg=msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     document.getElementById('statMaxZasOff').innerHTML=web;
     p1=p1+1;
     var msg=msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     document.getElementById('statMaxZasOn').innerHTML=web;
     p1=p1+1;
     var msg=msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     document.getElementById('statMaxKolektor').innerHTML=web;
     p1=p1+1;
     var msg=msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     
        
         
     if (web =="AUTO") {document.getElementById("CerpAuto").style.background = '#60A917';  VizCerp=1;document.images['Cerpadlo'].src = 'images/cerpadlo0.gif'; } 
    else 
                          {document.getElementById("CerpAuto").style.background = 'GRAY';} 
                                                    
     if (web =="ON")  {document.getElementById("CerpOn").style.background = '#60A917';if ( VizCerp==1) document.images['Cerpadlo'].src = 'images/cerpadloani2.gif'; VizCerp=0;} 
    else 
                          {document.getElementById("CerpOn").style.background = 'GRAY';}        
     
     if (web =="OFF")  {document.getElementById("CerpOff").style.background = '#60A917'; VizCerp=1;document.images['Cerpadlo'].src = 'images/cerpadlo0.gif'; } 
    else 
                          {document.getElementById("CerpOff").style.background = 'GRAY';} 
     
     
     
 //  }
 
   
}





if (web=="Cidla"){  
     p1=p1+1; 
     var msg = server_message.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     document.getElementById('kolektor').innerHTML=web;
     p1=p1+1;
     var msg =  msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     document.getElementById('zasdole').innerHTML=web;
     p1=p1+1;
     var msg=msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     document.getElementById('zasnahore').innerHTML=web;
     p1=p1+1;
     var msg=msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     document.getElementById('mereni1').innerHTML=web;
     p1=p1+1;
     var msg=msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     document.getElementById('mereni2').innerHTML=web;
     p1=p1+1;
     var msg=msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     document.getElementById('mereni3').innerHTML=web;
     p1=p1+1;
     var msg=msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     
     
     if (web =="STOP")  { if ( VizCerp==0) {document.images['Cerpadlo'].src = 'images/cerpadlo0.gif'; VizCerp=1; } }

                                                    
     if (web =="RUN") { if ( VizCerp==1){ document.images['Cerpadlo'].src = 'images/cerpadloani2.gif'; VizCerp=0;}} 
       

     p1=p1+1;
     var msg=msg.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);   
     document.getElementById('time').innerHTML=web;
     TimeLast= web;
     
   }



    if (web=="StatCid"){  
                 p1=p1+1; 
            var msg = server_message.slice(p1);
                 p1  =  msg.search("#");
                 web =  msg.slice(0,p1);
                  console.log(web) ; 
                   if (web=="neprirazen")  {PriradCid1=0;}
                   if (web=="kolektory")   {PriradCid1=1;} 
                   if (web=="zas. dole")   {PriradCid1=2;}
                   if (web=="zas. nahore") {PriradCid1=3;}
                   if (web=="mereni 1")    {PriradCid1=4;}
                   if (web=="mereni 2")    {PriradCid1=5;}
                   if (web=="mereni 3")    {PriradCid1=6;}
                   
                  p1=p1+1; 
              var msg = msg.slice(p1);
                  p1  =  msg.search("#");
                  web =  msg.slice(0,p1);
                  console.log(web) ; 
                   if (web=="neprirazen")  {PriradCid2=0;}
                   if (web=="kolektory")   {PriradCid2=1;} 
                   if (web=="zas. dole")   {PriradCid2=2;}
                   if (web=="zas. nahore") {PriradCid2=3;}
                   if (web=="mereni 1")    {PriradCid2=4;}
                   if (web=="mereni 2")    {PriradCid2=5;}
                   if (web=="mereni 3")    {PriradCid2=6;}

                 p1=p1+1; 
            var msg = msg.slice(p1);
                 p1  =  msg.search("#");
                 web =  msg.slice(0,p1);
                  console.log(web) ; 
                   if (web=="neprirazen")  {PriradCid3=0;}
                   if (web=="kolektory")   {PriradCid3=1;} 
                   if (web=="zas. dole")   {PriradCid3=2;}
                   if (web=="zas. nahore") {PriradCid3=3;}
                   if (web=="mereni 1")    {PriradCid3=4;}
                   if (web=="mereni 2")    {PriradCid3=5;}
                   if (web=="mereni 3")    {PriradCid3=6;}
                   
                  p1=p1+1; 
              var msg = msg.slice(p1);
                  p1  =  msg.search("#");
                  web =  msg.slice(0,p1);
                  console.log(web) ; 
                   if (web=="neprirazen")  {PriradCid4=0;}
                   if (web=="kolektory")   {PriradCid4=1;} 
                   if (web=="zas. dole")   {PriradCid4=2;}
                   if (web=="zas. nahore") {PriradCid4=3;}
                   if (web=="mereni 1")    {PriradCid4=4;}
                   if (web=="mereni 2")    {PriradCid4=5;}
                   if (web=="mereni 3")    {PriradCid4=6;}
                   
                  p1=p1+1; 
              var msg = msg.slice(p1);
                  p1  =  msg.search("#");
                  web =  msg.slice(0,p1);
                  console.log(web) ; 
                   if (web=="neprirazen")  {PriradCid5=0;}
                   if (web=="kolektory")   {PriradCid5=1;} 
                   if (web=="zas. dole")   {PriradCid5=2;}
                   if (web=="zas. nahore") {PriradCid5=3;}
                   if (web=="mereni 1")    {PriradCid5=4;}
                   if (web=="mereni 2")    {PriradCid5=5;}
                   if (web=="mereni 3")    {PriradCid5=6;}
                   
                  p1=p1+1; 
              var msg = msg.slice(p1);
                  p1  =  msg.search("#");
                  web =  msg.slice(0,p1);
                  console.log(web) ; 
                   if (web=="neprirazen")  {PriradCid6=0;}
                   if (web=="kolektory")   {PriradCid6=1;} 
                   if (web=="zas. dole")   {PriradCid6=2;}
                   if (web=="zas. nahore") {PriradCid6=3;}
                   if (web=="mereni 1")    {PriradCid6=4;}
                   if (web=="mereni 2")    {PriradCid6=5;}
                   if (web=="mereni 3")    {PriradCid6=6;}
                                        
                    
                  p1=p1+1; 
              var msg = msg.slice(p1);
                  p1  =  msg.search("#");
                  web =  msg.slice(0,p1);
                       console.log(web) ; 
                     
                        if(web=='1')  { PocetCidel=1; } 
                        if(web=='2')  { PocetCidel=2; } 
                        if(web=='3')  { PocetCidel=3; } 
                        if(web=='4')  { PocetCidel=4; }                                       
                        if(web=='5')  { PocetCidel=5; } 
                        if(web=='6')  { PocetCidel=6; }
                                                                                             }                                       
                                    
                                      
                                      
                           
                         
                            

    if (web=="MaxConnected"){   
                                document.cookie="ESPSESSIONID=0;"; console.log("odpoj");
                                connection.close(); 
                                window.location.href = '/login';
                                alert("Pripojeno prilis mnoho zarizeni zaroven");}


 if (web=="DefaultNastav"){   
 
    dialog_load.unload();      
    new Messi('<b>Navráceno do továrního nastavení</b>',{autoclose: 3000 ,width:270,closeButton:false});
    };




  if (web=="NaselPocetCidel"){
  
           dialog_load.unload();
     p1=p1+1; 
     var msg = server_message.slice(p1);
     p1  =  msg.search("#");
     web =  msg.slice(0,p1);
     
    dialog_load = new Messi('<b>Nalezeny '+web+' èidla</b>',{autoclose: 3000 ,width:150});


                       

                        if(web=="1")       { document.images['statcid1'].src = 'images/cidla1.png'; } 
                        if(web=="2")       { document.images['statcid1'].src = 'images/cidla1.png';
                                             document.images['statcid2'].src = 'images/cidla1.png'; } 
                        if(web=="3")       { document.images['statcid1'].src = 'images/cidla1.png';
                                             document.images['statcid2'].src = 'images/cidla1.png'; 
                                             document.images['statcid3'].src = 'images/cidla1.png'; } 
                        if(web=="4")       { document.images['statcid1'].src = 'images/cidla1.png';
                                             document.images['statcid2'].src = 'images/cidla1.png';
                                             document.images['statcid3'].src = 'images/cidla1.png';
                                             document.images['statcid4'].src = 'images/cidla1.png'; }                                       
                        if(web=="5")       { document.images['statcid1'].src = 'images/cidla1.png';
                                             document.images['statcid2'].src = 'images/cidla1.png';
                                             document.images['statcid3'].src = 'images/cidla1.png';
                                             document.images['statcid4'].src = 'images/cidla1.png';
                                             document.images['statcid5'].src = 'images/cidla1.png'; } 
                        if(web=="6")       { document.images['statcid1'].src = 'images/cidla1.png';
                                             document.images['statcid2'].src = 'images/cidla1.png';
                                             document.images['statcid3'].src = 'images/cidla1.png';
                                             document.images['statcid4'].src = 'images/cidla1.png';
                                             document.images['statcid5'].src = 'images/cidla1.png';
                                             document.images['statcid6'].src = 'images/cidla1.png'; }   



                             };            
    console.log(server_message) ;
  
      };
      

  
 
  // webové tlaèítka 
  function Butt_DiffOnUp() {
   
   butt = 'b00';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
     function Butt_DiffOnDw() {
   
   butt = 'b01';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
     function Butt_DiffOffUp() {
   
   butt = 'b02';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
     function Butt_DiffOffDw() {
   
   butt = 'b03';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
     function Butt_MaxZasOffUp() {
   
   butt = 'b04';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
     function Butt_MaxZasOffDw() {
   
   butt = 'b05';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
 
      function Butt_MaxZasOnUp() {
   
   butt = 'b06';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
     function Butt_MaxZasOnDw() {
   
   butt = 'b07';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
        function Butt_MaxKolektorUp() {
   
   butt = 'b08';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
     function Butt_MaxKolektorDw() {
   
   butt = 'b09';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
 
     function Butt_CerpAuto() {
     
   butt = 'b10';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
     function Butt_CerpOn() {
   
   butt = 'b11';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
     function Butt_CerpOff() {
   
   butt = 'b12';    
   console.log('BUTT: ' + butt);
   connection.send(butt); } 
   
     function Butt_DefaultNastav() {     dialog_load.unload();  dialog_load=new Messi(  '<IMG SRC="images/loading.gif">',{width:70,modal:true,closeButton:false} );
     
   butt = 'b13';    
   console.log('BUTT: ' + butt);
   connection.send(butt); 
  
   }   
   
   
   
   
     function Butt_HledejCidla() {
    dialog_load = new Messi(  '<IMG SRC="images/loading.gif">',{width:70,modal:true,closeButton:false} );
   butt = 'b14';    
   console.log('BUTT: ' + butt);
   connection.send(butt); }  
   
   
   
   
   
   
    function Butt_Cidlo1() {
   var sel_Cidlo1 = document.getElementById("Cidlo1");
           
                 if (sel_Cidlo1.selectedIndex ==0) { butt = 's00';} 
                 if (sel_Cidlo1.selectedIndex ==1) { butt = 's01';} 
                 if (sel_Cidlo1.selectedIndex ==2) { butt = 's02';} 
                 if (sel_Cidlo1.selectedIndex ==3) { butt = 's03';} 
                 if (sel_Cidlo1.selectedIndex ==4) { butt = 's04';} 
                 if (sel_Cidlo1.selectedIndex ==5) { butt = 's05';} 
                 if (sel_Cidlo1.selectedIndex ==6) { butt = 's06';} 
   console.log('BUTT: ' + butt);
   connection.send(butt); }   
   
   
       function Butt_Cidlo2() {
   var sel_Cidlo2 = document.getElementById("Cidlo2");
           
                 if (sel_Cidlo2.selectedIndex ==0) { butt = 's10';} 
                 if (sel_Cidlo2.selectedIndex ==1) { butt = 's11';} 
                 if (sel_Cidlo2.selectedIndex ==2) { butt = 's12';} 
                 if (sel_Cidlo2.selectedIndex ==3) { butt = 's13';} 
                 if (sel_Cidlo2.selectedIndex ==4) { butt = 's14';} 
                 if (sel_Cidlo2.selectedIndex ==5) { butt = 's15';} 
                 if (sel_Cidlo2.selectedIndex ==6) { butt = 's16';} 
     
   console.log('BUTT: ' + butt);
   connection.send(butt); }  
   
        function Butt_Cidlo3() {
   var sel_Cidlo3 = document.getElementById("Cidlo3");
           
                 if (sel_Cidlo3.selectedIndex ==0) { butt = 's20';} 
                 if (sel_Cidlo3.selectedIndex ==1) { butt = 's21';} 
                 if (sel_Cidlo3.selectedIndex ==2) { butt = 's22';} 
                 if (sel_Cidlo3.selectedIndex ==3) { butt = 's23';} 
                 if (sel_Cidlo3.selectedIndex ==4) { butt = 's24';} 
                 if (sel_Cidlo3.selectedIndex ==5) { butt = 's25';} 
                 if (sel_Cidlo3.selectedIndex ==6) { butt = 's26';} 
     
   console.log('BUTT: ' + butt);
   connection.send(butt); }  
  
  
        function Butt_Cidlo4() {
   var sel_Cidlo4 = document.getElementById("Cidlo4");
           
                 if (sel_Cidlo4.selectedIndex ==0) { butt = 's30';} 
                 if (sel_Cidlo4.selectedIndex ==1) { butt = 's31';} 
                 if (sel_Cidlo4.selectedIndex ==2) { butt = 's32';} 
                 if (sel_Cidlo4.selectedIndex ==3) { butt = 's33';} 
                 if (sel_Cidlo4.selectedIndex ==4) { butt = 's34';} 
                 if (sel_Cidlo4.selectedIndex ==5) { butt = 's35';} 
                 if (sel_Cidlo4.selectedIndex ==6) { butt = 's36';} 
     
   console.log('BUTT: ' + butt);
   connection.send(butt); }  
   
   
       function Butt_Cidlo5() {
   var sel_Cidlo5 = document.getElementById("Cidlo5");
           
                 if (sel_Cidlo5.selectedIndex ==0) { butt = 's40';} 
                 if (sel_Cidlo5.selectedIndex ==1) { butt = 's41';} 
                 if (sel_Cidlo5.selectedIndex ==2) { butt = 's42';} 
                 if (sel_Cidlo5.selectedIndex ==3) { butt = 's43';} 
                 if (sel_Cidlo5.selectedIndex ==4) { butt = 's44';} 
                 if (sel_Cidlo5.selectedIndex ==5) { butt = 's45';} 
                 if (sel_Cidlo5.selectedIndex ==6) { butt = 's46';} 
     
   console.log('BUTT: ' + butt);
   connection.send(butt); }  
  
  
         function Butt_Cidlo6() {
   var sel_Cidlo6 = document.getElementById("Cidlo6");
           
                 if (sel_Cidlo6.selectedIndex ==0) { butt = 's50';} 
                 if (sel_Cidlo6.selectedIndex ==1) { butt = 's51';} 
                 if (sel_Cidlo6.selectedIndex ==2) { butt = 's52';} 
                 if (sel_Cidlo6.selectedIndex ==3) { butt = 's53';} 
                 if (sel_Cidlo6.selectedIndex ==4) { butt = 's54';} 
                 if (sel_Cidlo6.selectedIndex ==5) { butt = 's55';} 
                 if (sel_Cidlo6.selectedIndex ==6) { butt = 's56';} 
     
   console.log('BUTT: ' + butt);
   connection.send(butt); }
   
   
   
             function dialog1(){
          var dialog1= TINY.box;
          dialog1.show({url:'NastavSolar.html',boxid:'frameless',width:400,height:290,fixed:false,maskid:'greymask',maskopacity:40,loading:0,left: '880px',top:'120px',mask:false,eschide:1,close_butt:1, })
           }
          function dialog2(){
          
                
          var dialog2= TINY1.box;
          dialog2.show({url:'NastavCidla.html',boxid:'frameless',width:400,height:260,fixed:false,maskid:'greymask',maskopacity:40,loading:0,left: '880px',top:'480px',mask:false,eschide:1,close_butt:1, })
                              
                    butt = 'a';    
                   console.log('BUTT: ' + butt);
                   connection.send(butt);
          
           }
           
           var zobraz=0; 
          function dialog3(){
          

          
          
         
          if (zobraz==0){
           dialog_load=new Messi(
          '<b>Opravdu chcete vratit tovarni nastaveni?</b>',
           {   width:310,
               closeButton:false,
               buttons: [
            {id: 0, label: 'ANO', val: 'Y'},
            {id: 1, label: 'NE',  val: 'N'},
                        ],
            callback: function(val){ if (val=='Y'){console.log(val);Butt_DefaultNastav();zobraz=0;}
                                     if (val=='N'){console.log(val);zobraz=0;} }});}
               zobraz=1;    
               
 }
               
                          function fce(){console.log("funguje");  document.getElementById("Cidlo1").selectedIndex=PriradCid1;
                                                                  document.getElementById("Cidlo2").selectedIndex=PriradCid2;
                                                                  document.getElementById("Cidlo3").selectedIndex=PriradCid3;
                                                                  document.getElementById("Cidlo4").selectedIndex=PriradCid4;
                                                                  document.getElementById("Cidlo5").selectedIndex=PriradCid5;
                                                                  document.getElementById("Cidlo6").selectedIndex=PriradCid6;
                                                                  
                        if(PocetCidel==1)  { document.images['statcid1'].src = 'images/cidla1.png'; } 
                        if(PocetCidel==2)  { document.images['statcid1'].src = 'images/cidla1.png';
                                             document.images['statcid2'].src = 'images/cidla1.png'; } 
                        if(PocetCidel==3)  { document.images['statcid1'].src = 'images/cidla1.png';
                                             document.images['statcid2'].src = 'images/cidla1.png'; 
                                             document.images['statcid3'].src = 'images/cidla1.png'; } 
                        if(PocetCidel==4)  { document.images['statcid1'].src = 'images/cidla1.png';
                                             document.images['statcid2'].src = 'images/cidla1.png';
                                             document.images['statcid3'].src = 'images/cidla1.png';
                                             document.images['statcid4'].src = 'images/cidla1.png'; }                                       
                        if(PocetCidel==5)  { document.images['statcid1'].src = 'images/cidla1.png';
                                             document.images['statcid2'].src = 'images/cidla1.png';
                                             document.images['statcid3'].src = 'images/cidla1.png';
                                             document.images['statcid4'].src = 'images/cidla1.png';
                                             document.images['statcid5'].src = 'images/cidla1.png'; } 
                        if(PocetCidel==6)  { document.images['statcid1'].src = 'images/cidla1.png';
                                             document.images['statcid2'].src = 'images/cidla1.png';
                                             document.images['statcid3'].src = 'images/cidla1.png';
                                             document.images['statcid4'].src = 'images/cidla1.png';
                                             document.images['statcid5'].src = 'images/cidla1.png';
                                             document.images['statcid6'].src = 'images/cidla1.png'; }   
                                                                  
                                                                  
                                                                  
                          }
   


   
 //  if (document.getElementById("Cidlo1").selectedIndex == document.getElementById("Cidlo2").selectedIndex){console.log("jede");}

   
   
   
   
   
   
   
//  };
  
//   });
  
//    }
  