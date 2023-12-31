#This is corusework made about: "modeling the operation of a pager communication system"
RTU MIREA, 1 course, by Shilov Pavel.


Надо моделировать работу систему связи пейджеров следующей конструкции.

Система состоит из следующих элементов:
  оператора системы;
  пульта управления;
  множества пейджеров (устройство для приема и передачи текста);
  множество пользователей;
  экрана отображения информации о функционировании системы.
  Правила функционирования системы:

Каждый пейджер имеет уникальный номер. Пользователь пейджера набирает номер абонента и текст сообщения.
Далее делает звонок оператору и передает сообщение.
Сообщение на устройстве оператора ставится в очередь.
Оператор передает текст сообщения из очереди согласно номеру абонента.
Система, моделирующая работу системы пейджеров, функционирует по тактам.

Максимальная длина передаваемого текста сообщения 30 знаков. За один такт пользователь может набрать от 1 до 10 символов. После набора номера пейджера абонента, пробела и текста сообщения нажимает на кнопку «Отправить». Нажатие на кнопку «Отправить» соответствует вводу одного символа. Набор текста всегда начинается в начале очередного такта.  Одновременно набор текста могут начать несколько пользователей.

Поступающие к оператору сообщения ставятся в очередь. Оператор в течении одного такта может передать одному абоненту одно сообщение.

Команды системы.

Команда запроса передачи сообщения:

Send_a_message «номер пейджера отправителя» «номер пейджера абонента» «текст сообщения»
Данная команда моделирует начало ввода сообщения пользователем от исходящего пейджера к абоненту. Исходя из длины сообщения отправка выполняется в рамках соответствующего такта. В конце отработки последовательно идущих команд (правило: одновременно набор текста могут начать несколько пользователей) элементы системы выполняют действия согласно такту. Если пейджер занят вводом, то новая поступившая команда запроса передачи сообщения, игнорируется.

Команда вывода списка отправленных и принятых сообщений пейджером:

Display the pager status «номер пейджера»
По данной команде выдается построчно список отправленных и принятых сообщений пейджером. Если в момент выдачи команды происходит ввод данных сообщения пейджера (пейджер занят), то выдача списка игнорируется. Списки упорядочены по номерам тактов отправки и получения сообщения. Если список пуст, то выводиться только заголовок. После отработки команды элементы системы выполняют действия согласно такту.

Команда выдачи состояния системы:

Display the system status
По данной команде выводиться состояние системы в текущем такте. Информация содержит перечень пейджеров. Относительно каждого пейджера сколько сообщений было передано и сколько принято. Пейджеры упорядочены по номерам. Относительно оператора выводиться: сколько запросов было обработано и сколько еще стоит в очереди. После отработки команды элементы системы выполняют действия согласно такту.

Пустая команда (строка ничего не содержит). Элементы системы выполняют действия согласно такту.

Команда завершения работы системы:

Turn off the system
Построить систему, которая использует объекты:

Объект «система».
Объект для чтения команд и данных. Объект моделирует работу пользователей пейджеров и оператора. Считывает данные для подготовки и настройки системы. После чтения очередной порции данных для настройки или данных команды, объект выдает сигнал с текстом полученных данных. Объект может выдать и другие сигналы. Все данные настройки и данные команд синтаксический корректны. Каждая строка команд соответствует одному такту (кроме последовательности команд запроса передачи сообщения). Если строка пустая, то система отрабатывает один такт.
Объект пульта управления моделирует работу оператора и его устройства, для отработки поступивших команд. Объект выдает соответствующий сигнал или сигналы. Управляет очередью, содержит список номеров пейджеров. Определяет корректность номера абонента в сообщении. Если номер набран некорректно, то выдает сигнал соответствующим сообщением. Готовит данные исходя из состояния системы. Оператор выбирает из очереди очередной запрос, формирует сообщение и отправляет абоненту.  Действие выполняется в рамках одного такта. Объект формирует сообщение для абонента по шаблону: 

«номер такта» Mail «номер пейджера абонента» «номер пейджера отправителя» «текст сообщения» ​
Объект, моделирующий пейджер. Фиксирует начало ввода сообщения. Выдает сигнал, содержащий команду отправления сообщения. Отправленные и полученные сообщения сохраняет в журнале (в памяти пейджера). По команде вывода списка отправленных сообщений выдает сигналы к устройству вывода с текстом отправленных сообщений.
Объект для вывода состояния или результата команды системы на консоль. Текст для вывода объект получает по сигналу от других объектов системы. Каждое присланное сообщение выводиться с новой строки.
Написать программу, реализующую следующий алгоритм:

Вызов метода объекта «система» build_tree_objects ( ).
  Построение дерева иерархии объектов. Характеристики объектов вводятся.
  Цикл для обработки вводимых данных и загрузки исходного состояния системы.
    Выдача сигнала объекту чтения для ввода данных.
    Отработка операции загрузки очередной порции данных.
    Установка связей сигналов и обработчиков с новым объектом.
  Установка связей сигналов и обработчиков между объектами.
Вызов метода объекта «система» exec_app ( ).
  Приведение всех объектов в состояние готовности.
  Цикл для обработки вводимых команд.
    Выдача сигнала объекту для ввода команды.
    Отработка команды.
  После ввода команды «Turn off the system» завершить работу.
  Все приведенные сигналы и соответствующие обработчики должны быть реализованы. Запрос от объекта означает выдачу сигнала. Все сообщения на консоль выводятся с новой строки.

В набор поддерживаемых команд добавить команду «SHOWTREE» и по этой команде вывести дерево иерархии объектов системы с отметкой о готовности и завершить работу системы (программы).

При решении задачи необходимо руководствоваться методическим пособием и приложением к методическому пособию

Входные данные

Ввод информации о пейджерах:
«номер пейджера»
Завершение информации о пейджерах:
End of information about pagers


Далее следуют команды. Каждая команда выполняется в рамках одного такта.
Последней командой является: Turn off the system
