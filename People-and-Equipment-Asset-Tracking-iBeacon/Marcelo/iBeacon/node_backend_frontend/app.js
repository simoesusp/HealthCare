var express = require('express');
var mysql = require('mysql');
var app = express();
var connection = mysql.createConnection({
host : 'localhost',
user : 'root',
password : 'admin',
dateStrings:true,
database: "LOCBLE"
});

app.use('/node_modules',  express.static(__dirname + '/node_modules'));

app.use('/style',  express.static(__dirname + '/style'));

app.use(express.static(__dirname + '/javascript'));

app.get('/',function(req,res){
	console.log("Got a GET request for the homepage");
    res.sendFile('home.html',{'root': __dirname + '/templates'});
})

// This responds a POST request for the homepage
app.post('/', function (req, res) {
   console.log("Got a POST request for the homepage");
   res.send('Hello POST');
})


app.post('/getlog', function (req, res) {
	//connection.connect();
	console.log("Connection!");
    connection.query('SELECT * FROM LOG WHERE TIMESTAMP  IN (SELECT MAX(TIMESTAMP) from LOG GROUP BY DEVICEID,MACADR)', function (error, rows, fields) {     
        // wrap result-set as json
        //json = JSON.stringify(rows);
		//connection.end();
		console.log('The query-result is: ', rows);
		res.send(rows);
      });
});

// Binding express app to port 3000
app.listen(3000,function(){
    console.log('Node server running @ http://localhost:3000')
});

app.post('/getlog', function (req, res) {
    console.log('works');
});
