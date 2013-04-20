CREATE SCHEMA stock_data
  AUTHORIZATION postgres;

-- Table: stock_data."DepthMarketData"

DROP TABLE IF EXISTS stock_data."DepthMarketData";

CREATE TABLE stock_data."DepthMarketData"
(
  "TradingDay" text NOT NULL,
  "InstrumentID" text NOT NULL,
  "ExchangeID" text NOT NULL,
  "ExchangeInstID" text NOT NULL,
  "LastPrice" double precision,
  "PreSettlementPrice" double precision,
  "PreClosePrice" double precision,
  "PreOpenInterest" double precision,
  "OpenPrice" double precision,
  "HighestPrice" double precision,
  "LowestPrice" double precision,
  "Volume" double precision,
  "Turnover" double precision,
  "OpenInterest" double precision,
  "ClosePrice" double precision,
  "SettlementPrice" double precision,
  "UpperLimitPrice" double precision,
  "LowerLimitPrice" double precision,
  "PreDelta" double precision,
  "CurrDelta" double precision,
  "UpdateTime" text NOT NULL,
  "UpdateMillisec" bigint NOT NULL,
  "BidPrice1" double precision,
  "BidVolume1" double precision,
  "AskPrice1" double precision,
  "AskVolume1" double precision,
  "BidPrice2" double precision,
  "BidVolume2" double precision,
  "AskPrice2" double precision,
  "AskVolume2" double precision,
  "BidPrice3" double precision,
  "BidVolume3" double precision,
  "AskPrice3" double precision,
  "AskVolume3" double precision,
  "BidPrice4" double precision,
  "BidVolume4" double precision,
  "AskPrice4" double precision,
  "AskVolume4" double precision,
  "BidPrice5" double precision,
  "BidVolume5" double precision,
  "AskPrice5" double precision,
  "AskVolume5" double precision,
  "AveragePrice" double precision,
  CONSTRAINT "primary" PRIMARY KEY ("TradingDay" , "InstrumentID" , "ExchangeID" , "ExchangeInstID" , "UpdateTime" , "UpdateMillisec" )
)
WITH (
  OIDS=FALSE
);
ALTER TABLE stock_data."DepthMarketData"
  OWNER TO postgres;

-- Table: stock_data."HalfMinuteData"

DROP TABLE IF EXISTS stock_data."HalfMinuteData";

CREATE TABLE stock_data."HalfMinuteData"
(
  day text,
  "time" text,
  open_price double precision,
  close_price double precision,
  high_price double precision,
  low_price double precision,
  volume double precision,
  open_interest double precision,
  instrument_id text
)
WITH (
  OIDS=FALSE
);
ALTER TABLE stock_data."HalfMinuteData"
  OWNER TO postgres;

-- Table: stock_data."OneMinuteData"

DROP TABLE IF EXISTS stock_data."OneMinuteData";

CREATE TABLE stock_data."OneMinuteData"
(
  day text NOT NULL,
  "time" text NOT NULL,
  open_price double precision,
  close_price double precision,
  high_price double precision,
  low_price double precision,
  volume double precision,
  open_interest double precision,
  instrument_id text
)
WITH (
  OIDS=FALSE
);
ALTER TABLE stock_data."OneMinuteData"
  OWNER TO postgres;

-- Table: stock_data."TenMinuteData"

DROP TABLE IF EXISTS stock_data."TenMinuteData";

CREATE TABLE stock_data."TenMinuteData"
(
  day text NOT NULL,
  "time" text NOT NULL,
  open_price double precision,
  close_price double precision,
  high_price double precision,
  low_price double precision,
  volume double precision,
  open_interest double precision,
  instrument_id text
)
WITH (
  OIDS=FALSE
);
ALTER TABLE stock_data."TenMinuteData"
  OWNER TO postgres;

-- Table: stock_data.history_minute_data

-- DROP TABLE stock_data.history_minute_data;

CREATE TABLE stock_data.history_minute_data
(
  trans_time timestamp without time zone,
  open double precision,
  high double precision,
  low double precision,
  close double precision,
  volume double precision,
  open_interest double precision
)
WITH (
  OIDS=FALSE
);
ALTER TABLE stock_data.history_minute_data
  OWNER TO postgres;

-- Table: stock_data.order_details

DROP TABLE IF EXISTS stock_data.order_details;

CREATE TABLE stock_data.order_details
(
  broker_id text,
  investor_id text,
  amount integer,
  price double precision,
  trans_day text,
  trans_time text,
  instrument_id text,
  is_buy smallint,
  is_open smallint
)
WITH (
  OIDS=FALSE
);
ALTER TABLE stock_data.order_details
  OWNER TO postgres;

-- Table: stock_data.trade_details

DROP TABLE IF EXISTS stock_data.trade_details;

CREATE TABLE stock_data.trade_details
(
  broker_id text,
  investor_id text,
  amount integer,
  price double precision,
  trans_day text,
  trans_time text,
  instrument_id text,
  is_buy smallint,
  is_open smallint
)
WITH (
  OIDS=FALSE
);
ALTER TABLE stock_data.trade_details
  OWNER TO postgres;