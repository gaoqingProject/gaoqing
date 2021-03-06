USE [gaoqing]
GO
/****** Object:  Table [dbo].[tbl_train_details]    Script Date: 02/25/2016 23:39:10 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[tbl_train_details](
	[car_id] [int] IDENTITY(1,1) NOT NULL,
	[tran_id] [int] NOT NULL,
	[car_speed] [int] NOT NULL,
	[car_type] [varchar](100) NOT NULL,
	[car_number] [varchar](50) NOT NULL,
	[car_time] [int] NOT NULL,
 CONSTRAINT [PK_tbl_train_details] PRIMARY KEY CLUSTERED 
(
	[car_id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[tbl_train]    Script Date: 02/25/2016 23:39:10 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[tbl_train](
	[tran_id] [int] IDENTITY(1,1) NOT NULL,
	[tran_name] [varchar](100) NOT NULL,
 CONSTRAINT [PK_tbl_train] PRIMARY KEY CLUSTERED 
(
	[tran_id] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Default [DF_tbl_train_details_tran_id]    Script Date: 02/25/2016 23:39:10 ******/
ALTER TABLE [dbo].[tbl_train_details] ADD  CONSTRAINT [DF_tbl_train_details_tran_id]  DEFAULT ((0)) FOR [tran_id]
GO
/****** Object:  Default [DF_tbl_train_details_speed]    Script Date: 02/25/2016 23:39:10 ******/
ALTER TABLE [dbo].[tbl_train_details] ADD  CONSTRAINT [DF_tbl_train_details_speed]  DEFAULT ((0)) FOR [car_speed]
GO
