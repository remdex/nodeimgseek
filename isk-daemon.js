var
  util = require('util'),
  http = require('http'),
  config = require('./settings'); 
     
// Load imgseek
var h = require("./imgseek");
var H = new h.ImgSeek();

// Load all databases
console.log('Loading databases - '+H.loadalldbs(config.database.path));

if (config.database.autosave == true)
{
    console.log('Autosave enabled...');
    setInterval(function(){
        console.log('Saving database...');
        H.savealldbs(config.database.path);
    },config.database.autosave_interval * 60 * 1000);
}

if (config.database.save_on_exit == true)
{
    console.log('Save database on exit enabled...');
    process.on('SIGINT', function () {
      console.log('Saving database on exit...');
      H.savealldbs(config.database.path);
      console.log('Database was saved...');
      process.exit(0);
    });
}

http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/plain'});
  
  var arguments = require('url').parse(req.url, true);  
  var ret = {};
  
  if (arguments.query.op == 'queryimgid') {
      
      if (arguments.query.dbid !== undefined && arguments.query.id !== undefined && arguments.query.nr !== undefined ){           
        ret = {'status' : true,'result' : H.queryimgid(arguments.query.dbid,arguments.query.id,arguments.query.nr)};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>, image id <id>,number of result <nr>"};
      }
      
  } else if (arguments.query.op == 'queryimgfile') {
      
      if (arguments.query.dbid !== undefined && arguments.query.fp !== undefined && arguments.query.nr !== undefined  && arguments.query.sk !== undefined ){           
        ret = {'status' : true,'result' : H.queryimgfile(arguments.query.fp, arguments.query.dbid, arguments.query.nr, arguments.query.sk)};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>, file path <fp>,number of result <nr>, is sketch 0|1 <sk>"};
      }
      
  } else if (arguments.query.op == 'queryimgfilefast') {
      
      if (arguments.query.dbid !== undefined && arguments.query.fp !== undefined && arguments.query.nr !== undefined  && arguments.query.sk !== undefined ){           
        ret = {'status' : true,'result' : H.queryimgfilefast(arguments.query.fp, arguments.query.dbid, arguments.query.nr, arguments.query.sk)};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>, file path <fp>,number of result <nr>, is sketch 0|1 <sk>"};
      }
      
  } else if (arguments.query.op == 'queryimgidfast') {
      
      if (arguments.query.dbid !== undefined && arguments.query.id !== undefined && arguments.query.nr !== undefined ){           
        ret = {'status' : true,'result' : H.queryimgidfast(arguments.query.dbid,arguments.query.id,arguments.query.nr)};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>, image id <id>, number of result <nr>"};
      }
      
  } else if (arguments.query.op == 'getimgcount') {
      
      if (arguments.query.dbid !== undefined){           
        ret = {'status' : true,'result' : H.getimgcount(arguments.query.dbid)};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>"};
      }
      
  } else if (arguments.query.op == 'removeid') {
      
      if (arguments.query.dbid !== undefined && arguments.query.id !== undefined){           
        ret = {'status' : true,'result' : H.removeid(arguments.query.dbid,arguments.query.id)};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>, image id <id>"};
      }
      
  } else if (arguments.query.op == 'isvaliddb') {
      
      if (arguments.query.dbid !== undefined){           
        ret = {'status' : true,'result' : H.isvaliddb(arguments.query.dbid)};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>"};
      }
      
  } else if (arguments.query.op == 'getdblist') {
      
      ret = {'result' : H.getdblist()};
      
  } else if (arguments.query.op == 'savealldbs') {
      
      ret = {'result' : H.savealldbs(config.database.path)};
      
  } else if (arguments.query.op == 'createdb') { 
      
      if (arguments.query.dbid !== undefined){           
        ret = {'result' : H.createdb( arguments.query.dbid )};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>"};
      }
      
  } else if (arguments.query.op == 'addimage') { 
      
      if (arguments.query.dbid !== undefined && arguments.query.id !== undefined && arguments.query.fp !== undefined){           
        ret = {'result' : H.addimage( arguments.query.dbid, arguments.query.id, arguments.query.fp )};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>, image id <id>, file path <fp>"};
      }
      
  } else if (arguments.query.op == 'isimageondb') {
      
      if (arguments.query.dbid !== undefined && arguments.query.id !== undefined){           
        ret = {'result' : H.isimageondb( arguments.query.dbid, arguments.query.id )};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>, image id <id>"};
      }
      
  } else if (arguments.query.op == 'removedb') { 
      
      if (arguments.query.dbid !== undefined) {           
        ret = {'status' : true,'result' : H.removedb( arguments.query.dbid )};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>"};
      }
      
  } else if (arguments.query.op == 'resetdb') { 
      
      if (arguments.query.dbid !== undefined){           
        ret = {'status' : true,'result' : H.resetdb( arguments.query.dbid )};
      } else {
        ret = {'status' : false,'msg' : "Please provide database id <dbid>"};
      }
      
  } else {
      ret = arguments;
  }

  res.end(JSON.stringify(ret));
  
}).listen(config.web.port, config.web.host);
console.log('Started server at http://'+config.web.host+':'+config.web.port+'/');