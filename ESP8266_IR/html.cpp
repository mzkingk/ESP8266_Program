#include <Arduino.h>

#include "html.h"

char *index_html = "<!DOCTYPE html><html lang='zh'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Document</title><style>.main{text-align:center}.tips{height:200px;display:flex;align-items:center;justify-content:center}.top-div{float:left;width:100%;height:100px}.top-div-1,.top-div-2,.top-div-3{text-align:center;float:left;padding:.5%;width:32%;height:100px}.bottom-div{border-radius:20px;margin:10px auto}.center{display:flex;align-items:center;justify-content:center}.circle{width:50px;height:50px;background:#fff;-moz-border-radius:50px;-webkit-border-radius:50px;border-radius:50px}.top-btn-word{font-size:35px}.input-area{width:80%}</style></head><body><div class='main'><h1>菜单</h1></div><div><div class='top-div center bottom-div'><div class='top-div-1 center' style='width:30%'>1.配置WiFi</div><div class='top-div-1 center' style='width:60%'><a href='/wifi'>进入页面</a></div></div><div class='top-div center bottom-div'><div class='top-div-1 center' style='width:30%'>2.遥控空调</div><div class='top-div-1 center' style='width:60%'><a href='/ir'>进入页面</a></div></div></div></body></html>";

char *wifi_html = "<!DOCTYPE html><html lang='zh'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Document</title><style>.main{text-align:center}.tips{height:200px;display:flex;align-items:center;justify-content:center}.top-div{float:left;width:100%;height:100px}.top-div-1,.top-div-2,.top-div-3{text-align:center;float:left;padding:.5%;width:32%;height:100px}.bottom-div{border-radius:20px;margin:10px auto}.center{display:flex;align-items:center;justify-content:center}.circle{width:50px;height:50px;background:#fff;-moz-border-radius:50px;-webkit-border-radius:50px;border-radius:50px}.top-btn-word{font-size:35px}.input-area{width:80%}</style></head><body><div class='main'><a href='/'><h1>wifi配置</h1></a></div><div><div class='top-div center bottom-div'><div class='top-div-1 center' style='width:30%'>1.WiFi名称</div><div class='top-div-1 center' style='width:60%'><input type='text' id='ssid' name='ssid' placeholder='手动输入wifi名' style='width:96%'></div></div><div class='top-div center bottom-div'><div class='top-div-1 center' style='width:30%'>2.选择WiFi</div><div class='top-div-1 center' style='width:60%'><select id='ssid2' style='width:100%' name='ssid2'></select></div></div><div class='top-div center bottom-div'><div class='top-div-1 center' style='width:30%'>WiFi密码</div><div class='top-div-1 center' style='width:60%'><input id='password' type='text' name='password' style='width:96%'></div></div><div onclick='saveData()' class='top-div center bottom-div'><input type='submit' value='提交'></div></div><script type='text/javascript'>function init(){ var oldValue=localStorage.getItem('ssid'); if (oldValue){ document.getElementById('ssid').value=oldValue;}} init(); getData(); function getData(){ var xhttp=new XMLHttpRequest(); xhttp.onreadystatechange=function (){ if (this.readyState==4 && this.status==200){ var list=JSON.parse(this.responseText); if (list){ var data=[]; try{ var dbms=list.map(x=>parseInt(x.substring(1, x.indexOf('dBm')))).sort((x, y)=>x - y); dbms.forEach(function (x){ var fd=list.filter(t=>t.indexOf('-' + x + 'dBm')===0); return data=data.concat(fd);}); data=[...new Set(data)];} catch (error){ data=[...new Set(list)];} var obj=document.getElementById('ssid2'); obj.options.length=0; for (let i=0; i < data.length; i++){ var name=data[i] ? data[i] : '-0dBm'; name=name.substring(name.indexOf('dBm') + 4); obj.options.add(new Option(data[i], name));}}}}; xhttp.open('GET', '/all', true); xhttp.send();} function saveData(){ var ssid=document.getElementById('ssid').value; var ssid2=document.getElementById('ssid2').value; var password=document.getElementById('password').value; var xhttp=new XMLHttpRequest(); xhttp.onreadystatechange=function (){ if (this.readyState==4 && this.status==200){ localStorage.setItem('ssid', ssid || ssid2); window.location.reload();}}; xhttp.open('POST', `/wifi?ssid=${ssid}&ssid2=${ssid2}&password=${password}`, true); xhttp.send();}</script></body></html>";

char *ir_html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Document</title><style>.main{text-align:center}.tips{height:200px;display:flex;align-items:center;justify-content:center}.top-div{float:left;width:100%;height:100px}.top-div-1,.top-div-2,.top-div-3{text-align:center;float:left;padding:.5%;width:32%;height:100px}.bottom-div{border-radius:20px;margin:10px auto}.center{display:flex;align-items:center;justify-content:center}.circle{width:50px;height:50px;background:#fcfdff;-moz-border-radius:50px;-webkit-border-radius:50px;border-radius:50px}.top-btn-word{font-size:35px}</style></head><body><div class='main'><a href='/'><h1>空调配置</h1></a></div><div class='tips' style='height:30px'><h5 id='text'></h5></div><div class='tips' style='height:30px'><h5 id='modeText'></h5></div><div class='tips' style='height:100px'><h3 id='status'>关闭</h3></div><div class='top-div'><div onclick='temperature(1)' class='top-div-1'><div>温度+</div><button class='circle top-btn-word'>+</button></div><div onclick='onOff()' class='top-div-2'><div>开关</div><button id='onOff' class='circle'>切换</button></div><div onclick='temperature(0)' class='top-div-3'><div>温度-</div><button class='circle top-btn-word'>-</button></div></div><div class='top-div center bottom-div' style='background:#fff0f5'><div onclick='modeChange()' class='top-div-1 center' style='width:40%'><button class='circle'>模式</button></div><div onclick='fanChange()' class='top-div-1 center' style='width:40%'><button class='circle'>风速</button></div></div><div class='top-div center bottom-div' style='background:#fff0f5'><div onclick='swingAuto()' class='top-div-1 center' style='width:40%'><button class='circle'>自动</button></div><div onclick='swingMode()' class='top-div-1 center' style='width:40%'><button class='circle'>扫风</button></div></div><script type='text/javascript'>var config={ status:{ value: false, url: '/ir/onOff'}, temperature:{ value: 26, url: '/ir/temperature',}, mode:{ value: '', text: '', url: '/ir/mode',}, fan:{ value: '', text: '', url: '/ir/fan',}, swingAuto:{ value: '', text: '', url: '/ir/swing-auto',}, swingMode:{ value: '', text: '', url: '/ir/swing-change',},}; var modeText=['自动', '冷风', '干燥', '换气', '暖风']; var fanText=['自动', '低', '中', '高']; var swingText=['kGreeSwingLastPos', 'kGreeSwingAuto', 'kGreeSwingUp', 'kGreeSwingMiddleUp', 'kGreeSwingMiddle', 'kGreeSwingMiddleDown', 'kGreeSwingDown', 'kGreeSwingDownAuto', 'kGreeSwingMiddleAuto', 'kGreeSwingUpAuto']; function onOff(){ handler(config.status.url);} function temperature(v){ handler(config.temperature.url + '?value=' + parseInt(v));} function modeChange(){ handler(config.mode.url);} function fanChange(){ handler(config.fan.url);} function swingAuto(){ handler(config.swingAuto.url);} function swingMode(){ handler(config.swingMode.url);} handler('/ir/config'); function handler(url, callback){ var xhttp=new XMLHttpRequest(); xhttp.onreadystatechange=function (){ if (this.readyState==4 && this.status==200){ var text=this.responseText; try{ var obj=JSON.parse(text); config.status.value=obj.status==='1'; config.temperature.value=obj.temperature; config.mode.value=obj.mode; config.mode.text=modeText[obj.mode]; config.fan.value=obj.fan; config.fan.text=fanText[obj.fan]; config.swingAuto.value=obj.swingAuto; config.swingMode.value=obj.swingMode; config.swingAuto.text=obj.swingAuto==='1' ? '摆风开' : '摆风关'; config.swingMode.text=swingText[obj.swingMode];} catch (error){ alert(text);} if (typeof callback==='function'){ callback();} statusChange();}}; xhttp.open('POST', url, true); xhttp.send();} function statusChange(){ document.getElementById('status').innerHTML=config.status.value ? '开启' : '关闭'; if (config.status.value){ var text=config.temperature.value + ' ℃;'; text +=` ${config.fan.text}`; text +=` ${config.swingAuto.text}-${config.swingMode.text}`; document.getElementById('text').innerHTML=text; document.getElementById('modeText').innerHTML=modeText[config.mode.value];;} else{ document.getElementById('text').innerHTML='';}}</script></body></html>";
