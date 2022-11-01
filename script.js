let button = document.querySelector("#btn")
let ledBtn = document.querySelector("#led-btn")

button.addEventListener('click', e => {
    // let bg = document.querySelector('body')
    

    // let picker = document.querySelector("#cp");
    // bg.style.background = picker.value;

    // let slider = document.querySelector("#sr");
    // let h1 = document.querySelector("h1");
    // h1.style.fontSize = slider.value;

    fetch("http://10.0.0.8:80/sensor").then((response) => {
        return response.json();
    })
    .then((data) => {
        console.log(data);
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
    postData("http://10.0.0.8:80/led", { "red": 42 })
        .then((data) => {
          console.log(data); // JSON data parsed by `data.json()` call
    });
});