const defaultQuat = new THREE.Quaternion(1, 0, 0, 0);
var scene;
var camera;
var renderer;
var rectangle;

const serviceId = "a0661d83-4017-4d5e-a7a9-330c74de60a3";
const characteristicId = "241ff4a0-c71f-4843-9c12-11e8cfea0079";
const textDecoder = new TextDecoder();
var characteristic;

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

document.getElementById("selectDeviceBtn").addEventListener("click", async (event) => {
	const device = await navigator.bluetooth.requestDevice({ 
		filters: [{ namePrefix: "VR-Shoe" }],
		optionalServices: [serviceId]
	});
	const server = await device.gatt.connect();
	const service = await server.getPrimaryService(serviceId);
	characteristic = await service.getCharacteristic(characteristicId);
	if(characteristicId) {
		characteristic.addEventListener("characteristicvaluechanged", sensorValuesChanged);
		setInterval(function () {
			characteristic.readValue()
		}, 300);
		console.log("Connected to device");
	}
});

function sensorValuesChanged(event) {
	const sensorValues = textDecoder.decode(event.target.value);
	console.log(sensorValues);
	const sensorValuesJson = JSON.parse(sensorValues);
	const quat = new THREE.Quaternion(
		sensorValuesJson.qx, 
		sensorValuesJson.qy, 
		sensorValuesJson.qz, 
		sensorValuesJson.qw
	);
    rectangle.quaternion.multiplyQuaternions(defaultQuat, quat);
}

initializeOrientationScene();