function addTask(event) {
    event.preventDefault();

    const title = document.getElementById("title").value.trim(); //zemi sto e vneseno kako naslov
    const content = document.getElementById("content").value.trim(); //zemi sto e vneseno kako string
    //trim e fja koja ignorira prazni mesta 

    if (title) {
        const taskList = document.getElementById("taskList"); //kreiraj lista

        // Создавање нови елементи
        const listItem = document.createElement("li"); //kreiraj nov list item 

        const strongTitle = document.createElement("strong"); //kreira vidliv naslov i go pravi da se razlikuva od sodrzinata
        strongTitle.textContent = title;

        const taskContent = document.createElement("span"); //span element e inline element koj ja sodrzi sodrzinata na zadacata
        taskContent.textContent = ":" + content;

        const deleteButton = document.createElement("button");
        deleteButton.textContent = "Избриши задача";

        // Поставување на функцијата за бришење
        deleteButton.onclick = function() {
            deleteTask(listItem); // Пренесуваме listItem (celiot element) za da go izbriseme
        };

        // Составување на елементите
        listItem.appendChild(strongTitle); //so kreiranje nna zadacata soodvetno se premesuvaat info i se sostavuvaat elementite 
        listItem.appendChild(taskContent);
        listItem.appendChild(deleteButton);
        taskList.appendChild(listItem);

        document.getElementById("title").value = ""; //da se iscisti formata za nov vlez 
        document.getElementById("content").value = "";
    }
}

// Функција за бришење на задача
function deleteTask(listItem) {
    listItem.remove(); // Бришење на целиот listItem (задача)
}