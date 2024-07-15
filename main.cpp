#include <iostream>
#include <pqxx/pqxx>
#include <pqxx/transaction>
#include <string>
#include <map>

class Baza
{
public:



    void create_table1(pqxx::connection& c)
    {
        std::string create1;
        std::string create2;


        try {
            create1 = "CREATE TABLE if not exists person ( id serial NOT NULL, "
                " first_name VARCHAR(80) NOT NULL, "
                "last_name VARCHAR(80) NOT NULL, "
                " e_mail VARCHAR(80) unique NOT NULL , "
                " CONSTRAINT person_pk PRIMARY KEY(id) "
                " ); ";
            create2 = "CREATE TABLE if not exists telefon ("
                "id_person int not NULL,"
                " telefon VARCHAR(12) unique not NULL,"
                " CONSTRAINT telefon_person_fk FOREIGN KEY(id_person) REFERENCES person(id)"
                " ); ";

            pqxx::work tx1{ c };
            tx1.exec(create1);



            tx1.exec(create2);


            tx1.commit();
            std::cout << "Удалось добавить таблицу person или она была уже создана" << std::endl;
            std::cout << "Удалось добавить таблицу telefon или она была уже создана" << std::endl;


        }
        catch (...)
        {
            std::cout << "Не удалось добавить таблицы person или telefon " << std::endl;
        }


    }
    void create_user(pqxx::connection& c, std::string first_name1, std::string last_name1, std::string e_mail1)
    {
        try {
            std::string create3;
            pqxx::transaction tx3{ c };

            create3 = "INSERT INTO person(first_name, last_name, e_mail) VALUES('" + first_name1 + "' , '" + last_name1 + "' , '" + e_mail1 + "');";
            //create3 = "INSERT INTO person2(first_name, last_name, e_mail) VALUES('Иван','IVa','rtyr45@ya.ru');";
            //std::cout << "Строка " << create3 << std::endl;

            //tx3.exec(create2);
            tx3.exec(create3);
            //tx3.exec(create4);
            //tx3.exec(create5);
            tx3.commit();
        }
        catch (...)
        {
            std::cout << "Не удалось добавить usera " << first_name1 << ", " << last_name1 << ", " << e_mail1 << std::endl;
        }


    }

    void add_telefon(pqxx::connection& c, std::string first_name1, std::string last_name1, std::string e_mail1, std::string telefon1)
    {
        int id1;
        std::string create2;
        std::string create1 = "select id from person where ((first_name='"
            "" + first_name1 + "') and (last_name ='"
            "" + last_name1 + "') and (e_mail ='"
            "" + e_mail1 + "')); ";
        //std::cout << "create1 " << create1 << std::endl;
        try
        {

            pqxx::work tx1{ c };
            id1 = tx1.query_value<int>(create1);
            //  std::cout << "id " << id1 << std::endl;
            create2 = "INSERT INTO telefon (id_person, telefon) values ('" + std::to_string(id1) + "','" + telefon1 + "');";
            //std::cout << "create1 " << create1 << std::endl;
            //std::cout << "create2 " << create2 << std::endl;
            tx1.exec(create2);
            tx1.commit();
        }
        catch (...)
        {
            std::cout << "Не удалось добавить телефон " << telefon1 << " для пользователя " << first_name1 << ", " << last_name1 << ", " << e_mail1 << std::endl;
        }
    }


    void update_user(pqxx::connection& c, std::string first_name1, std::string last_name1, std::string e_mail1,
        std::string first_name_new, std::string last_name_new, std::string e_mail1_new)
    {
        std::string create1 = "update person set first_name = '" + first_name_new + "', last_name ='" + last_name_new + "', e_mail = '" + e_mail1_new + "' "
            "where ((first_name='"
            "" + first_name1 + "') and (last_name ='"
            "" + last_name1 + "') and (e_mail ='"
            "" + e_mail1 + "')); ";

        //std::cout << "Запрос на изменение " << create1 << std::endl;
        try
        {
            pqxx::work tx1{ c };
            tx1.exec(create1);
            tx1.commit();
        }
        catch (...)
        {
            std::cout << "Не удалось заменить данные " << first_name1 << " ," << last_name1 << " ,"
                << e_mail1 << " на новые данные " << first_name_new << ", " << last_name_new << ", " << e_mail1_new << std::endl;
        }


    }

    void delete_telefon(pqxx::connection& c, std::string telefon1)
    {
        std::string create1 = "delete from telefon where (telefon='"+telefon1+"');";
        try
        {
            pqxx::work tx1{ c };
            tx1.exec(create1);
            tx1.commit();
        }
        catch (...)
        {
            std::cout << "Не удалось удалить телефон " << telefon1 << std::endl;
        }
    }


    void delete_user(pqxx::connection& c, std::string first_name1, std::string last_name1, std::string e_mail1)
    {

        int id1;
        std::string create2;
        std::string create3;
        std::string create1 = "select id from person where ((first_name='"
            "" + first_name1 + "') and (last_name ='"
            "" + last_name1 + "') and (e_mail ='"
            "" + e_mail1 + "')); ";
        try
        {
            pqxx::work tx1{ c };
            id1 = tx1.query_value<int>(create1);
            //  std::cout << "id " << id1 << std::endl;
            create2 = "delete from telefon where (id_person='" + std::to_string(id1) + "');";
            create3 = "delete from person where (id='" + std::to_string(id1) + "');";
            tx1.exec(create2);
            tx1.exec(create3);
            tx1.commit();
        }
        catch (...)
        {
            std::cout << "Не удалось удалить пользователя " << first_name1 << " ," << last_name1 << " ,"
                << e_mail1<< std::endl;

        }
    
    }

    int poisk(pqxx::connection& c, std::string first_name1, std::string last_name1, std::string e_mail1)
    {
        std::string create1 = "select id from person where ((first_name='"
            "" + first_name1 + "') and (last_name ='"
            "" + last_name1 + "') and (e_mail ='"
            "" + e_mail1 + "')); ";
        int id1;
        try
        {
            pqxx::work tx1{ c };
            id1 = tx1.query_value<int>(create1);
            tx1.commit();
            return id1;
        }
        catch (...)
        {
            std::cout << "Не удалось найти пользователя " << first_name1 << " ," << last_name1 << " ,"
                << e_mail1 << std::endl;
        }
    
    }

    int poisk(pqxx::connection& c, std::string telefon1)
    {
        int id1;
        std::string create1 = "select id_person from telefon where (telefon='" + telefon1 + "');";
        try
        {
            pqxx::work tx1{ c };
            id1 = tx1.query_value<int>(create1);
            tx1.commit();
            return id1;
        }
        catch (...)
        {
            std::cout << "Не удалось найти пользователя по телефону " << telefon1 << std::endl;
        }
    
    }


};

int main()
{



    setlocale(LC_ALL, "Russian");

   /* std::string vrem1;
    std::string put_podkl = "localhost";
    std::string port = "5432";
    std::string baza_dann = "my_database";
    std::string super_polzovatel = "postgres";
    std::string parol_super_polzovatel;


    std::cout << "Если хотите, чтобы были данные по умолчанию, а именно путь подключения: " << put_podkl << std::endl;
    std::cout << "Порт подключения: " << port << std::endl;
    std::cout << "База данных: " << baza_dann << std::endl;
    std::cout << "Имя суперпользователя: " << super_polzovatel << std::endl;
    std::cout << "Нажмите клавишу Y, а затем клавишу Enter: " << std::endl;
    std::cout << "Если хотите ввести ввести данные вручную, то нажмите любую другую клавишу, а затем клавишу Enter : ";
    std::cin >> vrem1;
    if ((vrem1 != "Y") && (vrem1 != "y"))
    {
        std::cout << "Введите путь подключения:";
        std::cin >> put_podkl;
        std::cout << std::endl;
        std::cout << "Введите порт подключения:";
        std::cin >> port;
        std::cout << std::endl;
        std::cout << "Введите имя базы данных:";
        std::cin >> baza_dann;
        std::cout << std::endl;
        std::cout << "Введите имя суперпользователя базы данных:";
        std::cin >> super_polzovatel;
        std::cout << std::endl;

    }
    std::cout << "Введите пароль суперпользователя базы данных:";
    std::cin >> parol_super_polzovatel;
    std::cout << std::endl;




    std::string vvod = "host=";


    vvod += put_podkl;
    vvod += " port=";
    vvod += port;
    vvod += " dbname=";
    vvod += baza_dann;
    vvod += " user=";

    vvod += super_polzovatel;
    vvod += " password=";
    vvod += parol_super_polzovatel;

    */
    std::string vvod="host=localhost "
        "port=5432 "
        "dbname=my_database "
        "user=postgres "
        "password=skipetr";

    Baza a1;
    try
    {
        pqxx::connection c(vvod);
        std::cout << "Удалось подключится" << std::endl;


        a1.create_table1(c);
        a1.create_user(c, "Иван", "Петров", "petrov1988@ya.ru");
        a1.add_telefon(c, "Иван", "Петров", "petrov1988@ya.ru", "2344577");
        //a1.update_user(c, "Иван", "Петров", "petrov1988@ya.ru", "Петр","Иванов","ivanov@yandex.ru");
        a1.create_user(c, "Андрей", "Петров", "petrov2024@ya.ru");
        a1.update_user(c, "Андрей", "Петров", "petrov2024@ya.ru", "Николай", "Иванов", "ivanov2024@yandex.ru");
        a1.delete_telefon(c, "2344577");
        a1.add_telefon(c, "Иван", "Петров", "petrov1988@ya.ru", "4567486786");
        a1.delete_user(c, "Иван", "Петров", "petrov1988@ya.ru");
        std::cout<<a1.poisk(c, "Николай", "Иванов", "ivanov2024@yandex.ru")<<std::endl;





        // ...
    }
    catch (pqxx::sql_error e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "Не удалось подключится к базе" << std::endl;
    }

   



}


/*
Создайте программу для управления клиентами на C++.
Нужно хранить персональную информацию о клиентах:
•	имя,
•	фамилия,
•	email,
•	телефон.
Сложность в том, что телефон у клиента может быть не один, а два, три и даже больше. А может и не быть — например, если он не захотел его оставлять.
Вам нужно разработать структуру БД для хранения информации и написать класс на С++ для управления данными со следующими методами:
1.	Метод, создающий структуру БД (таблицы).
2.	Метод, позволяющий добавить нового клиента.
3.	Метод, позволяющий добавить телефон для существующего клиента.
4.	Метод, позволяющий изменить данные о клиенте.
5.	Метод, позволяющий удалить телефон у существующего клиента.
6.	Метод, позволяющий удалить существующего клиента.
7.	Метод, позволяющий найти клиента по его данным — имени, фамилии, email или телефону.
Эти методы обязательны, но это не значит, что должны быть только они. Можно создавать дополнительные методы и классы.
Также предоставьте код, демонстрирующий работу написанного вами класса.
Подсказка
Не читайте этот раздел сразу, попытайтесь сначала решить задачу самостоятельно :)
Посмотреть методы
•	Чтобы создать таблицы, используйте метод transaction_base::exec, а в аргументах передавайте SQL запрос с "CREATE TABLE".
•	Чтобы добавить телефон клиента, используйте метод transaction_base::exec, а в аргументах передавайте SQL запрос с "INSERT".
•	Чтобы изменить данные о клиенте, используйте метод transaction_base::exec, а в аргументах передавайте SQL запрос с "UPDATE".
•	Чтобы удалить данные о клиенте или телефон, используйте метод transaction_base::exec, а в аргументах передавайте SQL запрос с "DELETE".
•	Для поиска клиента по его данным используйте метод transaction_base::query, а в аргументах передавайте SQL запрос с "SELECT".
*/