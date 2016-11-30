lock tables `Books` WRITE;
insert into `Books`values 
(1,'The C Programing Language',200,'Dannis Retchie','Engilsh','TsU Press','1978',28.5,'$',54,45,0,'C programmng classic textbook'),
/*insert into `Books`values */
(2,'计算机导论',1,'Brian Schanders','Engilsh','PH Press','1986',38.5,'$',34,25,0,'计算机基础');



unlock tables;
