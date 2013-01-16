var http = require('http');
var addon = require('./sv_addon');
var hello = addon.hello();  //调用了 sv_addon 模块中的 hello 方法

http.createServer(function (req, res) {

  res.writeHead(200, {'Content-Type': 'text/plain','charset':'UTF-8',});
  // console.log(hello);
  
  hello+= '\n node.js\n svdb addr:';
  hello+= addon.test(); //调用了 sv_addon 模块中的 svapi 方法，取得 svdb 的地址
  res.end(hello);

}).listen(1337, "127.0.0.1");
console.log('Server running at http://127.0.0.1:1337/');