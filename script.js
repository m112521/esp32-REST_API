let button = document.querySelector("#btn")
let ledBtn = document.querySelector("#led-btn")
let slider = document.querySelector("#sr")
let h1 = document.querySelector("h1")
let sliderNum = document.querySelector("#slider-num")

slider.addEventListener('change', e => {
    sliderNum.innerHTML = slider.value;
})

button.addEventListener('click', e => {
    // let bg = document.querySelector('body')
    

    // let picker = document.querySelector("#cp");
    // bg.style.background = picker.value;


    // 
    // h1.style.fontSize = slider.value;

    fetch("http://10.0.0.8:80/sensor").then((response) => {
        return response.json();
    })
    .then((data) => {
        console.log(data);
        h1.innerHTML = data.sensor;
    });
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
    postData("http://10.0.0.8:80/led", { "threshold": slider.value })
        .then((data) => {
          console.log(data); // JSON data parsed by `data.json()` call
    });
});