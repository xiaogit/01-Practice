drop table if exists `Books`;
create table `Books` (
`BookID` integer not null auto_increment primary key,
`BookName` varchar(128) not null,	/* 书名 */
`BookCategoryID` integer,	/* 类别号 */
`Author` varchar(128) not null,	/* 作者 */
`Language`varchar(16),	/* 语言 */
`PublishPress`varchar(128)not null,
`PublishDate` date not null,
`UnitPrice` float(7,3)not null,	/* stored up to 7 digits in total, and 3 digits may be after the  decimal point */
`CurrencyUnit` varchar(8)not null,	/* 币种usd,eud,rmb, */
`Inventory` integer not null,	/* 存量 */
`SalesVolume` integer not null,	/* 销量 */
`Discount` integer(2),	/* 折扣，常负，-10表示90% */
`Descriptions` varchar(256)
) CHARSET=gb18030;

drop table if exists `BookCategories`;
Create table `BookCategories`(
`BookCategoryID` integer not null auto_increment primary key,
`BookCategory` varchar(64) not null,
`FatherCategoryID` integer);

/* insert a category record */
lock tables `BookCategories` WRITE;
insert into `BookCategories` (BookCategoryID,BookCategory)values(1,'Computer Science');
unlock tables;
/**/

