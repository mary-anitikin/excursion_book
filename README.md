# Система регистрации экскурсионных поездок на разных видах транспорта
## excursion_book 
***
Проект создан с помощью фреймворка Qt Creator 4.6.2

Версия Qt 5.11.0

База данных SQLITE

ОС Astra Linux

***
Cистема регистрирует экскурсионные поездки на разных видах транспорта. Предоставляет возможность узнать сколько тот или иной транспорт сможет проехать на выданном количестве топлива.

На данный момент в системе существуют два типа транспорта: автомобили и самолеты.

У пользователя есть возможность добавить 
<ul>
    <li>тип транспорта</li>
    <li>конкретное транспортное средсво</li>
    <li>новую экскурсию</li>
</ul>

***

При запуске программы появляется окно с уведомлением о подключении к базе данных.

![alt text](https://github.com/mary-anitikin/excursion_book/blob/master/0.png?raw=true)

При успешном подключении открывается форма **"Регистрация экскурсионных поездок"**.

![alt text](https://github.com/mary-anitikin/excursion_book/blob/master/1.png?raw=true)

В таблице слева представлены доступные экскурсии.

В таблице справа - имеющиеся транспортные средства (ТС). 
В списке над таблицей транспортных средств можно выбирать тип транспортного средства (на данный момент это автомобиль или самолёт). 

<strong><em>Есть возможность удалять экскурсии и ТС.</em></strong>

Для добавления 
<ul>
    <li>экскурсии</li>
    <li>типа транспорта</li>
    <li>конкретного транспортного средсва</li>
</ul>
нажмите на кнопку <strong>"ДОБАВИТЬ"</strong>.


Откроется форма с полями для добавления.

![alt text](https://github.com/mary-anitikin/excursion_book/blob/master/2.png?raw=true)

<strong><em>Удалить тип транспортного средства можно на этой же форме, т.к. на ней располагается таблица с типами ТС.</em></strong>

![alt text](https://github.com/mary-anitikin/excursion_book/blob/master/011.gif?raw=true)


