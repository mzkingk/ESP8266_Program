#include <Arduino.h>

#include "html.h"

char *index_html = "<!DOCTYPE html><html lang='zh'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Document</title><style>.main{text-align:center}.tips{height:200px;display:flex;align-items:center;justify-content:center}.top-div{float:left;width:100%;height:100px}.top-div-1,.top-div-2,.top-div-3{text-align:center;float:left;padding:.5%;width:32%;height:100px}.bottom-div{border-radius:20px;margin:10px auto}.center{display:flex;align-items:center;justify-content:center}.circle{width:50px;height:50px;background:#fff;-moz-border-radius:50px;-webkit-border-radius:50px;border-radius:50px}.top-btn-word{font-size:35px}.input-area{width:80%}</style></head><body><div class='main'><h1>菜单</h1></div><div><div class='top-div center bottom-div'><div class='top-div-1 center' style='width:30%'>1.配置WiFi</div><div class='top-div-1 center' style='width:60%'><a href='/wifi'>进入页面</a></div></div><div class='top-div center bottom-div'><div class='top-div-1 center' style='width:30%'>2.遥控空调</div><div class='top-div-1 center' style='width:60%'><a href='/ir'>进入页面</a></div></div></div></body></html>";

char *wifi_html = "<!DOCTYPE html><html lang='zh'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Document</title><style>.main{text-align:center}.tips{height:200px;display:flex;align-items:center;justify-content:center}.top-div{float:left;width:100%;height:100px}.top-div-1,.top-div-2,.top-div-3{text-align:center;float:left;padding:.5%;width:32%;height:100px}.bottom-div{border-radius:20px;margin:10px auto}.center{display:flex;align-items:center;justify-content:center}.circle{width:50px;height:50px;background:#fff;-moz-border-radius:50px;-webkit-border-radius:50px;border-radius:50px}.top-btn-word{font-size:35px}.input-area{width:80%}</style></head><body><div class='main'><h1>wifi配置</h1></div><div><form name='input' action='/' method='POST'><div class='top-div center bottom-div'><div class='top-div-1 center' style='width:30%'>1.WiFi名称</div><div class='top-div-1 center' style='width:60%'><input type='text' name='ssid' placeholder='手动输入wifi名' style='width:96%'></div></div><div class='top-div center bottom-div'><div class='top-div-1 center' style='width:30%'>2.选择WiFi</div><div class='top-div-1 center' style='width:60%'><select id='ssid2' style='width:100%' name='ssid2'></select></div></div><div class='top-div center bottom-div'><div class='top-div-1 center' style='width:30%'>WiFi密码</div><div class='top-div-1 center' style='width:60%'><input type='text' name='password' style='width:96%'></div></div><div class='top-div center bottom-div'><input type='submit' value='提交'></div></form></div><script>getData(); function getData(){ var xhttp=new XMLHttpRequest(); xhttp.onreadystatechange=function (){ if (this.readyState==4 && this.status==200){ var list=JSON.parse(this.responseText); if (list){ var data=[]; try{ var dbms=list.map(x=>parseInt(x.substring(1, x.indexOf('dBm')))).sort((x, y)=>x - y); dbms.forEach(function (x){ var fd=list.filter(t=>t.indexOf('-' + x + 'dBm')===0); return data=data.concat(fd);}); data=[...new Set(data)];} catch (error){ data=[...new Set(list)];} var obj=document.getElementById('ssid2'); obj.options.length=0; for (let i=0; i < data.length; i++){ var name=data[i] ? data[i] : '-0dBm'; name=name.substring(name.indexOf('dBm') + 4); obj.options.add(new Option(data[i], name));}}}}; xhttp.open('GET', 'all', true); xhttp.send();}</script></body></html>";

char *ir_html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Document</title><style>.main{text-align:center}.tips{height:200px;display:flex;align-items:center;justify-content:center}.top-div{float:left;width:100%;height:100px}.top-div-1,.top-div-2,.top-div-3{text-align:center;float:left;padding:.5%;width:32%;height:100px}.bottom-div{border-radius:20px;margin:10px auto}.center{display:flex;align-items:center;justify-content:center}.circle{width:50px;height:50px;background:#fcfdff;-moz-border-radius:50px;-webkit-border-radius:50px;border-radius:50px}.top-btn-word{font-size:35px}</style></head><body><div class='main'><h1>空调配置页</h1></div><div class='tips'>关闭</div><div class='top-div'><div class='top-div-1'><div>温度+</div><button class='circle top-btn-word'>+</button></div><div class='top-div-2'><div>开关</div><button class='circle'>切换</button></div><div class='top-div-3'><div>温度-</div><button class='circle top-btn-word'>-</button></div></div><div class='top-div center bottom-div' style='background:#fff0f5'><div class='top-div-1 center' style='width:40%'><button class='circle'>+</button></div><div class='top-div-1 center' style='width:40%'><button class='circle'>+</button></div></div><div class='top-div center bottom-div' style='background:#fff0f5'><div class='top-div-1 center' style='width:40%'><button class='circle'>+</button></div><div class='top-div-1 center' style='width:40%'><button class='circle'>+</button></div></div></body></html>";
