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

copy stock_data.history_minute_data from 'c:\\if2011-2012-1min.csv' delimiter ',';
copy stock_data.history_minute_data from 'c:\\if2010-2011-1min.csv' delimiter ',';

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

truncate stock_data."OneMinuteData";
insert into stock_data."OneMinuteData" select substring(trans_time::text from 1 for 10), substring(trans_time::text from 12 for 8), open, close, high,
low, volume, open_interest, 'IF1207' from stock_data.history_minute_data;

