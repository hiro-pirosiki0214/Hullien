document.getElementById("save").onclick = function()
{
	var workbook = XLSX.utils.table_to_book(document.getElementById('file'));
	var name = workbook.SheetNames[0];
	
	var files = document.getElementById('file'), f = files[0];
	var reader = new FileReader();
	reader.onload = function(e) {
	var data = new Uint8Array(e.target.result);
	var workbook = XLSX.read(data, {type: 'array'});

	/* DO SOMETHING WITH workbook HERE */
	};
	reader.readAsArrayBuffer(f);
	var i = 0;
};

var canvas = document.getElementById("myCanvas");
var ctx = canvas.getContext("2d");

class CBullet
{
	constructor( x, y )
	{
		this.x			= x;
		this.y			= y;
		this.f_w		= canvas.width;
		this.f_h		= canvas.height;
		this.isShot		= false;
		this.moveSpeed	= 0.0;
		this.vec_x		= 0.0;
		this.vec_y		= 0.0;
	}
	
	shoot( rot, s ){
		if( this.isShot == true ) return;
		this.isShot = true;
		this.moveSpeed = s;
		this.vec_x = Math.sin(rot);
		this.vec_y = Math.cos(rot);
	}
	
	update( ctx )
	{
//		if( this.isShot == false ) return;
		this.x += this.vec_x * this.moveSpeed;
		this.y += this.vec_y * this.moveSpeed;
		
		ctx.beginPath();
		ctx.arc( this.x, this.y, 3, 360*Math.PI/180.0, false );
		ctx.fillStyle = "red";
		ctx.fill();
		ctx.closePath();
		
		return;
	}
}

var isStart = false;
var no = 0;
var text = 0;
var textsize = 0;
var hp = 0;
var moveSpeed = 0;
var bulletSpeed = 1;
var bulletNum = 30;
var nWayNum = 3;
var angle = 1;
var nWayAngle = 2;
var intrval = 10;
var dips = false;

document.getElementById("no").value = no;
document.getElementById("text").value = text;
document.getElementById("textsize").value = textsize;
document.getElementById("hp").value = hp;
document.getElementById("moveSpeed").value = moveSpeed;
document.getElementById("bulletSpeed").value = bulletSpeed;
document.getElementById("bulletNum").value = bulletNum;
document.getElementById("nWayNum").value = nWayNum;
document.getElementById("angle").value = angle;
document.getElementById("nWayAngle").value = nWayAngle;
document.getElementById("intrval").value = intrval;
document.getElementById("dips").value = dips;

var b = [];
var shotFrame = 0;
var shotCount = 0;
var shotAngle = 0;

function main()
{
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	document.getElementById("stop").onclick = function(){
		isStart = false;
	}
	document.getElementById("play").onclick = function(){
		//------------------------------------------.
		// パラメータの取得.
		no			= document.getElementById("no").value;
		text		= document.getElementById("text").value;
		textsize	= document.getElementById("textsize").value;
		hp			= Number(document.getElementById("hp").value);
		moveSpeed	= Number(document.getElementById("moveSpeed").value);
		bulletSpeed	= Number(document.getElementById("bulletSpeed").value);
		bulletNum	= Number(document.getElementById("bulletNum").value);
		nWayNum		= Number(document.getElementById("nWayNum").value);
		angle		= Number(document.getElementById("angle").value) * (Math.PI/180.0);
		nWayAngle	= Number(document.getElementById("nWayAngle").value) * (Math.PI/180.0);
		intrval		= Number(document.getElementById("intrval").value);
		dips		= Number(document.getElementById("dips").value);
		
		// 弾の初期化.
		for( var i = 0; i < bulletNum; i++ ){
			b[i] = new CBullet( 240, 100 );
		}
		isStart = true;
		shotFrame = 0;
		shotCount = 0;
		shotAngle = 0;
	}
	
	// スタートしていた場合弾などの更新をする.
	if( isStart == true ){
		//------------------------------------------.
		if( shotFrame % intrval == 0 ){
			var n;
			var a = shotAngle;
			for( n = 0; n < nWayNum; n++ ){
				if( shotCount < bulletNum ){
					b[shotCount].shoot( a, bulletSpeed );
					a += nWayAngle;
				} else {
					if( shotCount != bulletNum ){
						isStart = false;
						alert("弾数とnWay数が合いません");
					}
					break;
				}
				shotCount++;
			}
			shotAngle += angle;	// 角度の加算.
		}
		//------------------------------------------.
		
		// 弾の更新.
		for( var i = 0; i < shotCount; i++ ){
			b[i].update( ctx );
		}
		// フレームが一定値に達したら終了.
		if( shotFrame == (intrval*bulletNum) + 100 ){
			isStart = false;
		}
		shotFrame++;
	}
	
	//----------------------------------------.
	// デバッグテキスト.
	ctx.font = "16px Arial";
	ctx.fillStyele = "#0095DD";
	ctx.fillText(no,			8, 20 );
	ctx.fillText(text,			8, 40 );
	ctx.fillText(textsize,		8, 60 );
	ctx.fillText(hp,			8, 80 );
	ctx.fillText(moveSpeed,		8, 100 );
	ctx.fillText(bulletSpeed,	8, 120 );
	ctx.fillText(bulletNum,		8, 140 );
	ctx.fillText(nWayNum,		8, 160 );
	ctx.fillText(angle,			8, 180 );
	ctx.fillText(nWayAngle,		8, 200 );
	ctx.fillText(intrval,		8, 220 );
	ctx.fillText(dips,			8, 240 );
}

var interval = setInterval( main, 10 );