[![Queue](https://github.com/pernillamoreno/NewAss/actions/workflows/Queue.yml/badge.svg)](https://github.com/pernillamoreno/NewAss/actions/workflows/Queue.yml)

# My grade: VGAssignment-Ci
In this assignment, by practicing CI you are supposed to use TDD, googletest and a 
Makefile to test and develop a template circular queue class implemented 
using a singly linked list according to the requirements below.

    1.There shall be a Makefile to build and run the test *
        The generated files shall be stored in a directory named build *
        There shall be a rule to clean the project.*
        The default target shall build the test. *
        There shall be a rule to run the *
        
    2.Make a private repository and follow the Github Flow strategy

    3.Make a workflow which is triggered on

        Push to any branch in order to build and run the test *
        Pull request to main in order to build and run the test *

    4.Create a status badge in the README.md file of your repo.

    5.The template circular queue class shall have a template type (T) parameter 
    for type of data stored in a queue. 4

    6.Make the class uncopyable by deleteing he copy constructor and the copy assignment operator 1

    7.Make the class movable by implementing the move constructor and the move assignment operator 1

    8.Size of a queue is specified during runtime when the queue is created
        Size shall be greater than 2
        Size is passed to the constructor of the class
        The nodes shall be created and linked in the constructor

  

    9.The class shall have a function to resize a queue
        The new size shall also be greater than 2
        In the case you need to delete nodes which hold data elements, nodes with the oldest data 
        elements shall be deleted.

    10.The class shall have a function to make a queue empty

    11.The class shall have a function to read data from a queue

    12.The class shall have a function to write data to a queue
       If the queue is full then the oldest data shall be overwritten

    13.The class shall have a function to return number of data elements stored in a queue

    14.The class shall have a function to check if a queue is full or not

    15.If the template type(T) is an arithmetic type then the class shall have a 
       function to return average of the data elements stored in a queue.
        The function shall not change the queue
        The return type of the function shall be double.

    16.The class shall be tested and developed using TDD, gtest and gmock
        Ensure that the dynamically allocated memory blocks are released properly 
        Test the class for int, float and std::string types *
        To get VG, the class shall be tested fully.
