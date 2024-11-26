document.querySelector('input[type="submit"]').addEventListener('click', function(e) {
    e.preventDefault();
    
    let name = document.getElementById('name').value;
    let surname = document.getElementById('surname').value;
    let email = document.getElementById('email').value;
    let password = document.getElementById('password').value;
    let address = document.getElementById('address').value;
    let phone = document.getElementById('phone').value;
    
    if (!name || !surname || !email || !password || !address || !phone) {
        alert('Мора да ја пополните формата!');
        return;
    }
    
    // Проверка на форматот на телефонскиот број
    let phonePattern = /^\d{6}-\d{3}$/;
    if (!phonePattern.test(phone)) {
        alert('Телефонскиот број мора да биде во форматот 000000-000');
        return;
    }
    
    alert('Успешно ја пополнивте формата!');
});