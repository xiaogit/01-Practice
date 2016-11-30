/* 用户列表结构 */
drop table if exists `Accounts`;
create table `Accounts`(
`UserID` integer not null auto_increment primary key,	/* ID */
`UserName` varchar(32) not null,		/* 用户名 */
`Email` varchar(32)not null,	/* 邮箱 */
`Password`varchar(32)not null,	/* 密码 */
`Gender` varchar(16),		/* 性别 */
`Birthday` Date,		/* 生日 */
`CellPhone` varchar(32)not null,	/* 手机号码 */
`Address` varchar(128)not null	/* 地址 */
);

/* 发货地址列表 */
drop table if exists `DeliveryDests`;
create table `DeliveryDest`(
`DestID`integer not null auto_increment primary key,
`UserID`integer not null,
`Destination` varchar(128)not null,
`SerialNumber`integer not null,	/* 用户清单中的序号 */
`ZipCode` varchar(16)	/* 邮编 */ 
);

