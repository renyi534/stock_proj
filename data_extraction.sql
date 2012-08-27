DROP FUNCTION IF EXISTS  generate_market_data
    (
    TEXT,
    TEXT,
    TEXT,
    TEXT,
    TEXT,
    TEXT,
    TEXT
    );
CREATE OR REPLACE FUNCTION generate_market_data
    ( 
    original_table_name     TEXT,
    target_table_name       TEXT,
    s_trading_day             TEXT,
    e_trading_day             TEXT,
    future_num              TEXT,
    start_time              TEXT,
    final_time              TEXT
    ) 
RETURNS VOID AS $$
DECLARE
    index   BIGINT;
    result  BIGINT;
    stmt    TEXT;
BEGIN
    EXECUTE 'DROP TABLE IF EXISTS '||target_table_name||';';
    stmt= 'CREATE TABLE '||target_table_name||' AS 
        SELECT "LastPrice" as LastPrice,"TradingDay","UpdateTime","UpdateMillisec",
    avg("LastPrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 120 PRECEDING AND CURRENT ROW) as avg_lastprice1,
    avg("LastPrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 240 PRECEDING AND CURRENT ROW) as avg_lastprice2,                        
    avg("LastPrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_lastprice3,
    avg("LastPrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 1200 PRECEDING AND CURRENT ROW) as avg_lastprice4,                        
    avg("PreSettlementPrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_PreSettlementPrice, 
    avg("PreClosePrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_PreClosePrice, 
    avg("OpenPrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_OpenPrice,
    max("HighestPrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as max_HighestPrice, 
    min("LowestPrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as min_LowestPrice, 
    avg(volume) over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_Volume, 
    avg(turnover) over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_Turnover, 
    avg(openinterest) over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_OpenInterest, 
    avg("UpperLimitPrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_UpperLimitPrice, 
    avg("LowerLimitPrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_LowerLimitPrice, 
    avg("BidPrice1") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_BidPrice1, 
    avg("BidVolume1") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_BidVolume1, 
    avg("AskPrice1") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_AskPrice1, 
    avg("AskVolume1") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_AskVolume1, 
    avg("AveragePrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 600 PRECEDING AND CURRENT ROW) as avg_AveragePrice,   
    avg("LastPrice") over 
        (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec" 
                        ROWS BETWEEN 1 FOLLOWING AND 240 FOLLOWING) as avg_predprice
    from
(select *,"Volume"-lag("Volume") over (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec") AS volume,"OpenInterest"-lag("OpenInterest") over (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec") AS openinterest,"Turnover"-lag("Turnover") over (partition by "TradingDay","InstrumentID","ExchangeID","ExchangeInstID" ORDER BY  "TradingDay","UpdateTime","UpdateMillisec") AS turnover from '||original_table_name||' where "UpdateTime">='''||start_time||''' AND "UpdateTime"<='''||final_time||''' AND "TradingDay">='''||s_trading_day||''' AND "TradingDay"<='''||e_trading_day||''' and "InstrumentID"='''||future_num||''') l;';
    
    RAISE INFO '%',stmt;
    EXECUTE stmt;

    EXECUTE 'ALTER TABLE '||target_table_name||' ADD column class TEXT;';
    
    EXECUTE 'UPDATE '||target_table_name||' 
        SET class=
            CASE WHEN(avg_predprice>LastPrice+2) THEN
                ''Buy''
            ELSE
                CASE WHEN (avg_predprice<LastPrice-2) THEN
                    ''Sell''   
                ELSE
                    ''Hold''
                END
            END;';

    EXECUTE 'UPDATE '||target_table_name||' 
        SET 
          avg_lastprice1           = avg_lastprice1/LastPrice, 
          avg_lastprice2           = avg_lastprice2/LastPrice, 
          avg_lastprice3           = avg_lastprice3/LastPrice, 
          avg_lastprice4           = avg_lastprice4/LastPrice, 
          avg_presettlementprice  = avg_presettlementprice/LastPrice, 
          avg_precloseprice       = avg_precloseprice/LastPrice ,
          avg_openprice           = avg_openprice/LastPrice ,
          max_highestprice        = max_highestprice/LastPrice ,
          min_lowestprice         = min_lowestprice/LastPrice ,
          avg_upperlimitprice     = avg_upperlimitprice/LastPrice ,
          avg_lowerlimitprice     = avg_lowerlimitprice/LastPrice ,
          avg_bidprice1           = avg_bidprice1/LastPrice ,
          avg_askprice1           = avg_askprice1/LastPrice ,
          avg_averageprice        = avg_averageprice/LastPrice ;';
    EXECUTE 'ALTER TABLE '||target_table_name||' DROP column avg_predprice;';
    EXECUTE 'ALTER TABLE '||target_table_name||' DROP column LastPrice;';
END
$$ LANGUAGE PLPGSQL;




DROP FUNCTION IF EXISTS  export_table_to_csv
    (
    TEXT,
    TEXT
    );
CREATE OR REPLACE FUNCTION export_table_to_csv
    ( 
    original_table_name     TEXT,
    csv_file_name           TEXT    
    ) 
RETURNS VOID AS $$
DECLARE
    index   BIGINT;
    result  BIGINT;
    stmt    TEXT;
BEGIN
    EXECUTE 'COPY '||original_table_name||' to '''||csv_file_name||''' delimiter '','';';
END
$$ LANGUAGE PLPGSQL;


select generate_market_data('stock_data."DepthMarketData"','data_120329_11_1130','20120329','20120329','IF1204','11:00:00','11:30:00');
 select export_table_to_csv('data_120329_11_1130','/data/reny2/data_120329_11_1130.csv');
select n1.id,tradingday,UpdateTime,UpdateMillisec,n2.class from data_120329_11_1130 n1, classify_result n2 where n1.id=n2.id order by updatetime,updatemillisec;
 create table classify_res as select n1.id,tradingday,UpdateTime,UpdateMillisec,n2.class from data_120329_11_1130 n1, classify_result n2 where n1.id=n2.id order by updatetime,updatemillisec;
select export_table_to_csv('classify_res','/data/reny2/classify_res.csv');

psql -d madlib -c "select rf_test('data_120329_9_11','gini','avg_lastprice1,avg_lastprice2,avg_lastprice3,avg_lastprice4,avg_presettlementprice,avg_precloseprice,avg_openprice,max_highestprice,min_lowestprice,avg_volume,avg_turnover,avg_openinterest,avg_upperlimitprice,avg_lowerlimitprice,avg_bidprice1,avg_bidvolume1,avg_askprice1,avg_askvolume1,avg_averageprice','avg_lastprice1,avg_lastprice2,avg_lastprice3,avg_lastprice4,avg_presettlementprice,avg_precloseprice,avg_openprice,max_highestprice,min_lowestprice,avg_volume,avg_turnover,avg_openinterest,avg_upperlimitprice,avg_lowerlimitprice,avg_bidprice1,avg_bidvolume1,avg_askprice1,avg_askvolume1,avg_averageprice','data_120329_11_1130',10,'explicit',10,'f')" 

CREATE OR REPLACE FUNCTION generate_history_stat_data
    ( 
    original_table_name     TEXT,
    target_table_name       TEXT
    ) 
RETURNS VOID AS $$
DECLARE
    index   BIGINT;
    result  BIGINT;
    stmt    TEXT;
BEGIN
    EXECUTE 'DROP TABLE IF EXISTS '||target_table_name||';';
    stmt= 'CREATE TABLE '||target_table_name||' AS 
        SELECT trans_time, close, open/close as open, high/close as high, low/close as low, volume, open_interest, 
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 5 PRECEDING AND CURRENT ROW) as avg_close_5,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 10 PRECEDING AND CURRENT ROW) as avg_close_10,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 20 PRECEDING AND CURRENT ROW) as avg_close_20,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 40 PRECEDING AND CURRENT ROW) as avg_close_40,	
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 80 PRECEDING AND CURRENT ROW) as avg_close_80,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 160 PRECEDING AND CURRENT ROW) as avg_close_160,	
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 30 FOLLOWING AND 30 FOLLOWING) as avg_close_pred
    from '||original_table_name||';';
    
    RAISE INFO '%',stmt;
    EXECUTE stmt;

    EXECUTE 'ALTER TABLE '||target_table_name||' ADD column class TEXT;';
    
    EXECUTE 'UPDATE '||target_table_name||' 
        SET class=
            CASE WHEN(avg_close_pred>close+20) THEN
                ''Buy''
            ELSE
                CASE WHEN (avg_close_pred<close-20) THEN
                    ''Sell''   
                ELSE
                    ''Hold''
                END
            END;';

    EXECUTE 'UPDATE '||target_table_name||' 
        SET 
          avg_close_5           = avg_close_5/close, 
          avg_close_10           = avg_close_10/close,  
          avg_close_20           = avg_close_20/close, 
          avg_close_40           = avg_close_40/close,  
          avg_close_80           = avg_close_80/close,  
          avg_close_160           = avg_close_160/close  
       	;';
    EXECUTE 'ALTER TABLE '||target_table_name||' DROP column avg_close_pred;';
    EXECUTE 'ALTER TABLE '||target_table_name||' DROP column close;';
END
$$ LANGUAGE PLPGSQL;

CREATE OR REPLACE FUNCTION generate_history_stat_data
    ( 
    original_table_name     TEXT,
    target_table_name       TEXT,
    parameter	            FLOAT8
    ) 
RETURNS VOID AS $$
DECLARE
    index   BIGINT;
    result  BIGINT;
    stmt    TEXT;
BEGIN
    EXECUTE 'DROP TABLE IF EXISTS '||target_table_name||';';
    stmt= 'CREATE TABLE '||target_table_name||' AS 
        SELECT trans_time, close, open/close as open, high/close as high, low/close as low, volume, open_interest, 
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 5 PRECEDING AND CURRENT ROW) as avg_close_5,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 10 PRECEDING AND CURRENT ROW) as avg_close_10,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 20 PRECEDING AND CURRENT ROW) as avg_close_20,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 40 PRECEDING AND CURRENT ROW) as avg_close_40,	
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 80 PRECEDING AND CURRENT ROW) as avg_close_80,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 160 PRECEDING AND CURRENT ROW) as avg_close_160,	
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 30 FOLLOWING AND 30 FOLLOWING) as avg_close_pred
    from '||original_table_name||';';
    
    RAISE INFO '%',stmt;
    EXECUTE stmt;

    EXECUTE 'ALTER TABLE '||target_table_name||' ADD column class TEXT;';
    
    EXECUTE 'UPDATE '||target_table_name||' 
        SET class=
            CASE WHEN(avg_close_pred>close+'||parameter||') THEN
                ''Buy''
            ELSE
                CASE WHEN (avg_close_pred<close-'||parameter||') THEN
                    ''Sell''   
                ELSE
                    ''Hold''
                END
            END;';

    EXECUTE 'UPDATE '||target_table_name||' 
        SET 
          avg_close_5           = avg_close_5/close, 
          avg_close_10           = avg_close_10/close,  
          avg_close_20           = avg_close_20/close, 
          avg_close_40           = avg_close_40/close,  
          avg_close_80           = avg_close_80/close,  
          avg_close_160           = avg_close_160/close  
       	;';
    EXECUTE 'ALTER TABLE '||target_table_name||' DROP column avg_close_pred;';
    EXECUTE 'ALTER TABLE '||target_table_name||' DROP column close;';
END
$$ LANGUAGE PLPGSQL;


CREATE OR REPLACE FUNCTION generate_history_stat_data
    ( 
    original_table_name     TEXT,
    target_table_name       TEXT,
    parameter	            FLOAT8
    ) 
RETURNS VOID AS $$
DECLARE
    index   BIGINT;
    result  BIGINT;
    stmt    TEXT;
    ti      TIMESTAMP;
BEGIN
    ti=clock_timestamp();
    EXECUTE 'DROP TABLE IF EXISTS temp_history_tr_table';
    stmt= 'CREATE TEMP TABLE temp_history_tr_table AS
		SELECT trans_time, open, high, low, close, volume, open_interest,
		    CASE WHEN ( abs(high-low)>abs(high-last_close) ) THEN
			CASE WHEN (abs(high-low)>abs(low-last_close)) THEN
				abs(high-low)
			ELSE
				abs(low-last_close)
			END
		    ELSE
			CASE WHEN (abs(high-last_close)>abs(low-last_close)) THEN
				abs(high-last_close)
			ELSE
				abs(low-last_close)
			END
		    END as tr 
		FROM
			(SELECT trans_time, open, high, low, close, volume, open_interest,
		    		lag(close) over (ORDER BY trans_time) as last_close
			FROM '||original_table_name||') l;';
    RAISE INFO '%',stmt;
    EXECUTE stmt;
    RAISE INFO 'Time:%', clock_timestamp()-ti;

    ti=clock_timestamp();
    EXECUTE 'DROP TABLE IF EXISTS '||target_table_name||';';
    stmt= 'CREATE TABLE '||target_table_name||' AS 
        SELECT trans_time, close, open/close as open, high/close as high, low/close as low, volume, open_interest, tr, 
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 5 PRECEDING AND CURRENT ROW) as avg_close_5,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 10 PRECEDING AND CURRENT ROW) as avg_close_10,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 20 PRECEDING AND CURRENT ROW) as avg_close_20,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 40 PRECEDING AND CURRENT ROW) as avg_close_40,	
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 80 PRECEDING AND CURRENT ROW) as avg_close_80,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 160 PRECEDING AND CURRENT ROW) as avg_close_160,	
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 240 PRECEDING AND CURRENT ROW) as avg_close_240,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 5 PRECEDING AND CURRENT ROW) as atr_5,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 10 PRECEDING AND CURRENT ROW) as atr_10,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 20 PRECEDING AND CURRENT ROW) as atr_20,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 40 PRECEDING AND CURRENT ROW) as atr_40,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 80 PRECEDING AND CURRENT ROW) as atr_80,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 160 PRECEDING AND CURRENT ROW) as atr_160,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 240 PRECEDING AND CURRENT ROW) as atr_240,	
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 30 FOLLOWING AND 30 FOLLOWING) as avg_close_pred
    from temp_history_tr_table;';
    
    RAISE INFO '%',stmt;
    EXECUTE stmt;
    RAISE INFO 'Time:%', clock_timestamp()-ti;

    EXECUTE 'ALTER TABLE '||target_table_name||' ADD column class TEXT;';
    
    EXECUTE 'UPDATE '||target_table_name||' 
        SET class=
            CASE WHEN(avg_close_pred>close+'||parameter||') THEN
                ''Buy''
            ELSE
                CASE WHEN (avg_close_pred<close-'||parameter||') THEN
                    ''Sell''   
                ELSE
                    ''Hold''
                END
            END;';

    EXECUTE 'UPDATE '||target_table_name||' 
        SET 
          avg_close_5           = avg_close_5/close, 
          avg_close_10           = avg_close_10/close,  
          avg_close_20           = avg_close_20/close, 
          avg_close_40           = avg_close_40/close,  
          avg_close_80           = avg_close_80/close,  
          avg_close_240           = avg_close_240/close,  
          avg_close_160           = avg_close_160/close  
       	;';
    EXECUTE 'ALTER TABLE '||target_table_name||' DROP column avg_close_pred;';
    EXECUTE 'ALTER TABLE '||target_table_name||' DROP column close;';
END
$$ LANGUAGE PLPGSQL;


CREATE OR REPLACE FUNCTION generate_history_stat_data_dyn
    ( 
    original_table_name     TEXT,
    target_table_name       TEXT,
    parameter               FLOAT8
    ) 
RETURNS VOID AS $$
DECLARE
    index   BIGINT;
    result  BIGINT;
    stmt    TEXT;
    ti      TIMESTAMP;
BEGIN
    ti=clock_timestamp();
    EXECUTE 'DROP TABLE IF EXISTS temp_history_tr_table';
    stmt= 'CREATE TEMP TABLE temp_history_tr_table AS
		SELECT trans_time, open, high, low, close, volume, open_interest,
		    CASE WHEN ( abs(high-low)>abs(high-last_close) ) THEN
			CASE WHEN (abs(high-low)>abs(low-last_close)) THEN
				abs(high-low)
			ELSE
				abs(low-last_close)
			END
		    ELSE
			CASE WHEN (abs(high-last_close)>abs(low-last_close)) THEN
				abs(high-last_close)
			ELSE
				abs(low-last_close)
			END
		    END as tr 
		FROM
			(SELECT trans_time, open, high, low, close, volume, open_interest,
		    		lag(close) over (ORDER BY trans_time) as last_close
			FROM '||original_table_name||') l;';
    RAISE INFO '%',stmt;
    EXECUTE stmt;
    RAISE INFO 'Time:%', clock_timestamp()-ti;

    ti=clock_timestamp();
    EXECUTE 'DROP TABLE IF EXISTS '||target_table_name||';';
    stmt= 'CREATE TABLE '||target_table_name||' AS 
        SELECT trans_time, close, open/close as open, high/close as high, low/close as low, volume, open_interest, tr, 
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 5 PRECEDING AND CURRENT ROW) as avg_close_5,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 10 PRECEDING AND CURRENT ROW) as avg_close_10,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 20 PRECEDING AND CURRENT ROW) as avg_close_20,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 40 PRECEDING AND CURRENT ROW) as avg_close_40,	
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 80 PRECEDING AND CURRENT ROW) as avg_close_80,
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 160 PRECEDING AND CURRENT ROW) as avg_close_160,	
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 240 PRECEDING AND CURRENT ROW) as avg_close_240,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 5 PRECEDING AND CURRENT ROW) as atr_5,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 10 PRECEDING AND CURRENT ROW) as atr_10,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 20 PRECEDING AND CURRENT ROW) as atr_20,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 40 PRECEDING AND CURRENT ROW) as atr_40,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 80 PRECEDING AND CURRENT ROW) as atr_80,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 160 PRECEDING AND CURRENT ROW) as atr_160,	
    avg(tr) over 
        (ORDER BY trans_time ROWS BETWEEN 240 PRECEDING AND CURRENT ROW) as atr_240,	
    avg(close) over 
        (ORDER BY trans_time ROWS BETWEEN 30 FOLLOWING AND 30 FOLLOWING) as avg_close_pred
    from temp_history_tr_table;';
    
    RAISE INFO '%',stmt;
    EXECUTE stmt;
    RAISE INFO 'Time:%', clock_timestamp()-ti;

    EXECUTE 'ALTER TABLE '||target_table_name||' ADD column class TEXT;';
    
    EXECUTE 'UPDATE '||target_table_name||' 
        SET class=
            CASE WHEN(avg_close_pred>close+(atr_240*'||parameter||')) THEN
                ''Buy''
            ELSE
                CASE WHEN (avg_close_pred<close-(atr_240*'||parameter||')) THEN
                    ''Sell''   
                ELSE
                    ''Hold''
                END
            END;';

    EXECUTE 'UPDATE '||target_table_name||' 
        SET 
          avg_close_5           = avg_close_5/close, 
          avg_close_10           = avg_close_10/close,  
          avg_close_20           = avg_close_20/close, 
          avg_close_40           = avg_close_40/close,  
          avg_close_80           = avg_close_80/close,  
          avg_close_240           = avg_close_240/close,  
          avg_close_160           = avg_close_160/close  
       	;';
    EXECUTE 'ALTER TABLE '||target_table_name||' DROP column avg_close_pred;';
    EXECUTE 'ALTER TABLE '||target_table_name||' DROP column close;';
END
$$ LANGUAGE PLPGSQL;


CREATE OR REPLACE FUNCTION generate_train_data
    ( 
    original_table_name     TEXT,
    train_table		    TEXT,
    test_table		    TEXT,
    sep_time	            TEXT
    ) 
RETURNS VOID AS $$
DECLARE
    index   BIGINT;
    result  BIGINT;
    stmt    TEXT;
BEGIN
    EXECUTE 'DROP TABLE IF EXISTS '||train_table||';';
    stmt= 'CREATE TABLE '||train_table||' AS 
       SELECT * FROM 
    '||original_table_name||' WHERE trans_time<='''||sep_time||''';';
    RAISE INFO '%',stmt;
    EXECUTE stmt;
    
    EXECUTE 'DROP TABLE IF EXISTS '||test_table||';';
    stmt= 'CREATE TABLE '||test_table||' AS 
       SELECT * FROM 
    '||original_table_name||' WHERE trans_time>'''||sep_time||''';';
    RAISE INFO '%',stmt;
    EXECUTE stmt;

    EXECUTE 'ALTER TABLE '||test_table||' DROP column trans_time;';
    EXECUTE 'ALTER TABLE '||train_table||' DROP column trans_time;';
END
$$ LANGUAGE PLPGSQL;

CREATE OR REPLACE FUNCTION generate_history_stat_data
    ( 
    original_table_name     TEXT,
    target_table_name       TEXT
    ) 
RETURNS VOID AS $$
DECLARE
    index   BIGINT;
    result  BIGINT;
    stmt    TEXT;
BEGIN
    EXECUTE 'DROP TABLE IF EXISTS '||target_table_name||';';
    stmt= 
    'CREATE TABLE '||target_table_name||' AS 
     SELECT time::text, week_close as week_close, week_high, week_low, week_close/week_open as week_gain,
	week_open/prev_week_open as prev_week_gain, prev_week_open/prev2_week_open as prev2_week_gain,
	week_volume, pre_week_volume, week4_volume,
	CASE WHEN(close>open) THEN
		1
	ELSE
		0
	END::TEXT AS class
     FROM (
        SELECT time, close, open, lag(close) over (order by time) as week_close,
    	max(high) over 
        	(ORDER BY time ROWS BETWEEN 6 PRECEDING AND 1 PRECEDING) as week_high,
    	min(low) over 
        	(ORDER BY time ROWS BETWEEN 6 PRECEDING AND 1 PRECEDING) as week_low,
    	avg(open) over 
        	(ORDER BY time ROWS BETWEEN 6 PRECEDING AND 6 PRECEDING) as week_open,
    	avg(open) over 
        	(ORDER BY time ROWS BETWEEN 11 PRECEDING AND 11 PRECEDING) as prev_week_open,	
   	avg(open) over 
        	(ORDER BY time ROWS BETWEEN 16 PRECEDING AND 16 PRECEDING) as prev2_week_open,		
    	avg(volume) over 
        	(ORDER BY time ROWS BETWEEN 6 PRECEDING AND 1 PRECEDING) as week_volume,
    	avg(volume) over 
        	(ORDER BY time ROWS BETWEEN 11 PRECEDING AND 7 PRECEDING) as pre_week_volume,
    	avg(volume) over 
        	(ORDER BY time ROWS BETWEEN 21 PRECEDING AND 1 PRECEDING) as week4_volume	
    	from '||original_table_name||') l
     WHERE prev2_week_open IS NOT NULL;';
    
    RAISE INFO '%',stmt;
    EXECUTE stmt;
END
$$ LANGUAGE PLPGSQL;

create view day_data_stat_view as select row_number() over (order by time) as id, time, week_close, week_high, week_low, week_gain, prev_week_gain, prev2_week_gain, week_volume, pre_week_volume, week4_volume, class from day_stat_data;

