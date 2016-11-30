/* 插入基本的分类条目 */

lock tables `BookCategories` WRITE;
insert into `BookCategories` (BookCategory)values('Electric Engineering');
insert into `BookCategories` (BookCategory)values('Electronic Engineering');
insert into `BookCategories` (BookCategory)values('Telecommunication Engineering');
insert into `BookCategories` (BookCategory)values('Material Engineering');
insert into `BookCategories` (BookCategory,FatherCategoryID)values('Artificial Intelegience',1);

insert into `BookCategories` (BookCategory,FatherCategoryID,BookCategoryID)values('Operating Systems',1,100);
insert into `BookCategories` (BookCategory,FatherCategoryID,BookCategoryID)values('Artificial Intelegience',1,101); 
insert into `BookCategories` (BookCategory,FatherCategoryID,BookCategoryID)
  values ('Database  Principles',1,102);
insert into `BookCategories` (BookCategory,FatherCategoryID,BookCategoryID)
values('Compiler Principles',1,103);

insert into `BookCategories` (BookCategory,FatherCategoryID,BookCategoryID)
values('Programming Languages',1,104);

insert into `BookCategories` (BookCategory,FatherCategoryID,BookCategoryID)
valuse('C Programming Language',104,200);
insert into `BookCategories` (BookCategory,FatherCategoryID,BookCategoryID)
values('C Plus Plus Programming Language',104,201);
insert into `BookCategories` (BookCategory,FatherCategoryID,BookCategoryID)
values('Java Programming Language',104,202);
insert into `BookCategories` (BookCategory,FatherCategoryID,BookCategoryID)
values('Swift Programming Language',104,203);

insert into `BookCategories` (BookCategory,FatherCategoryID,BookCategoryID)
values('Computer Networks',1,105);


unlock tables;
