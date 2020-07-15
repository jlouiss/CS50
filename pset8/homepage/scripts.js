function typeWrite(element, speed = 90) {
    const text = element.dataset.typewriter;
    let index = 0;

    if (!text) {
        return;
    }

    const typewriterAnimationInterval = setInterval(() => {
        element.innerHTML += text[index];
        index++;

        if(index === text.length) {
          clearInterval(typewriterAnimationInterval);
        }
    }, speed);
}

window.onload = () => {
    const elements = document.querySelectorAll('[data-typewriter]');
    elements.forEach(element => typeWrite(element))
}