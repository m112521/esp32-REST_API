let button = document.querySelector("#btn");
let ledBtn = document.querySelector("#led-btn");
let slider = document.querySelector("#sr");
let sensorVal = document.querySelector("#sensor-val");
let sliderNum = document.querySelector("#slider-num");
let picker = document.querySelector("#cp");

const URL = "http://192.168.31.210:80";

function hexToRgb(hex) {
    let result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
      r: parseInt(result[1], 16),
      g: parseInt(result[2], 16),
      b: parseInt(result[3], 16)
    } : null;
}
  
slider.addEventListener('change', e => {
    sliderNum.innerHTML = slider.value;
})

button.addEventListener('click', e => {
    // fetch( `${URL}/sensor`).then((response) => {
    //     return response.json();
    // })
    // .then((data) => {
    //     console.log(data);
    //     sensorVal.innerHTML = data.sensor;
    // });

    // continuos update
    setInterval(()=> {
        fetch( `${URL}/sensor`).then((response) => {
            return response.json();
        })
        .then((data) => {
            //console.log(data);
            sensorVal.innerHTML = data.sensor;
        });
    }, 1000);

})

async function postData(url = '', data = {}) {
    const response = await fetch(url, {
      method: 'POST', 
      mode: 'no-cors', 
      cache: 'no-cache',
      credentials: 'same-origin', 
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(data) 
    });
    return response //.json(); //
}


ledBtn.addEventListener('click', e => {
    let rgb = hexToRgb(picker.value);
    console.log(rgb)
    postData(`${URL}/led`, { "threshold": slider.value, "R": rgb.r, "G": rgb.g, "B": rgb.b})
        .then((data) => {
          console.log(data); // JSON data parsed by `data.json()` call
    });
});