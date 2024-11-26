const shop = ['apple', 'banana', 'pear', 'strawberry'];

document.getElementById('shopTable').style.display = 'none';

function addItem() {
    let productName = document.getElementById('productName').value.toLowerCase();
    let quantity = document.getElementById('quantity').value;
    let price = document.getElementById('price').value;
    
    if (!shop.includes(productName)) {
        window.alert('Производот ' + productName + ' не постои во кошничката!');
        return;
    }

    document.getElementById('shopTable').style.display = 'table';

    let total = quantity * price;
    
    let table = document.getElementById('shopTable');
    let row = table.insertRow(-1);
    
    row.insertCell(0).textContent = productName;
    row.insertCell(1).textContent = quantity;
    row.insertCell(2).textContent = price;
    row.insertCell(3).textContent = total;
    
    let deleteCell = row.insertCell(4);
    let deleteImg = document.createElement('img');
    deleteImg.src = 'X.png';
    deleteImg.onclick = function() {
        table.deleteRow(row.rowIndex);
    };
    deleteCell.appendChild(deleteImg);
    
    // Ресетирање на полињата
    document.getElementById('productName').value = '';
    document.getElementById('quantity').value = '';
    document.getElementById('price').value = '';
}