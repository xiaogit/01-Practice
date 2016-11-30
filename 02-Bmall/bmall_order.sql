drop table if exists `PurchaseOrders`;
create table `PurchaseOrders`(
`OrderID` bigint not null auto_increment primary key, /* 订单号 */
`UserID` integer not null, 	/* 用户 */
/*`PurchaseListID`integer not null,		 /* 购买清单 */  
`PurchaseAmount` tinyint(1) not null,    	/* 购买数量 */
`TotalPrice` float(10,2)not null,	 /* 总价 */
`CurrencyUnit` varchar(8)not null,	/* 币种 */
`PurchaseTime` datetime not null,	/* 下单时间 */
`State` BIT(8),		/*有无付款，发货状态，是否完结（0/1--closed/open）b'11110000' */
`PayMethod`varchar(16),	/* 付款方式 */
`DestID` integer not null,
`CellPhone`varchar(32)not null,
`DeliveryForm`varchar(16)	/* shipment/express/air/etc.. */
);
/*
drop table if exists `PurchaseListID`;
create table `PurchaseListID`(
`OrderID` bigint not null auto_increment primary key,
`PurchaseListID` bigint not null auto_increment, /* 清单号 */
/*
`UserID` integer not null,
`BookID` integer not null,
`Amount` unsigned tinyint not null,	/* 购买数量 */
/*
); */
