const defaultQuat = new THREE.Quaternion(1, 0, 0, 0);
var scene;
var camera;
var renderer;
var rectangle;

const messageTerminator = "\r\n";
const baudRate = 115200;
const textDecoder = new TextDecoder();
var port;
var reader;
var keepReadingSerial = true;
var messageQueue = "";


function initializeOrientationScene() {
	const container = document.getElementById("orientationScene");

	scene = new THREE.Scene();
	camera = new THREE.PerspectiveCamera(
		75, 
		container.clientWidth / container.clientHeight, 
		0.1, 
		1000
	);
	renderer = new THREE.WebGLRenderer();

	renderer.setSize(container.clientWidth, container.clientHeight);
	container.appendChild(renderer.domElement);

	rectangle = new THREE.Mesh(
        new THREE.BoxGeometry(5, 1, 4),
		new THREE.MeshNormalMaterial()
	);
	scene.add(rectangle);

	camera.position.set(4, 5.15, 4);
	camera.lookAt(0, 0, 0);

	render();

	console.log("scene initialized");
}

function render() {
	requestAnimationFrame(render);
	renderer.render(scene, camera);
}

function checkForUsbSerial() {
	if (!("serial" in navigator)) {
		console.log("USB serial not supported by this browser");
	}
	else {
		console.log("USB serial supported by this browser");
	}
}

document.getElementById("selectPortBtn").addEventListener("click", async (event) => {
	port = await navigator.serial.requestPort();
	await port.open({ baudRate: baudRate });
	while(port.readable && keepReadingSerial) {
		reader = port.readable.getReader();
		try {
			while(keepReadingSerial) {
				await readUsbSerial();
			}
		}
		catch(error) {
			console.error(error);
		}
	}
});

async function readUsbSerial() {
	if(!port.readable) {
		console.log("Port not readable");
		keepReadingSerial = false;
		return;
	}
	const { value, done } = await reader.read();
	if(done) {
		console.log("Serial port was closed");
		reader.releaseLock();
		keepReadingSerial = false;
		return;
	};

	const message = textDecoder.decode(value);
	messageQueue += message;
	if(!messageQueue.includes(messageTerminator)) {
		return;
	}
	const nextMessage = messageQueue.slice(0, messageQueue.indexOf(messageTerminator));
	messageQueue = messageQueue.replace(nextMessage, "").trimStart();
	processSensorData(nextMessage);  
}


function processSensorData(serialMessage) {
	console.log(serialMessage);
	if(isJson(serialMessage)) {
		const sensorValuesJson = JSON.parse(serialMessage);
	    if(sensorValuesJson.qx && sensorValuesJson.qy && sensorValuesJson.qz && sensorValuesJson.qw) {
	    	const quat = new THREE.Quaternion(
	    		sensorValuesJson.qx, 
	    		sensorValuesJson.qy, 
	    		sensorValuesJson.qz, 
	    		sensorValuesJson.qw
	    	);
	    	rectangle.quaternion.multiplyQuaternions(defaultQuat, quat);
	    }
	}
}

function isJson(str) {
    try {
        JSON.parse(str);
    } catch (e) {
        return false;
    }
    return true;
}

initializeOrientationScene();
checkForUsbSerial();