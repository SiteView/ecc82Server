var http = require('http');
var addon = require('./sv_addon');
var hello = addon.hello();  //������ sv_addon ģ���е� hello ����

http.createServer(function (req, res) {

  res.writeHead(200, {'Content-Type': 'text/plain','charset':'UTF-8',});
  // console.log(hello);
  
  hello+= '\n node.js\n svdb addr:';
  hello+= addon.test(); //������ sv_addon ģ���е� svapi ������ȡ�� svdb �ĵ�ַ
  res.end(hello);

}).listen(1337, "127.0.0.1");
console.log('Server running at http://127.0.0.1:1337/');