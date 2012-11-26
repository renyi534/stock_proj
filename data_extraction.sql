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

create view day_data_stat_view_hs300 as select row_number() over (order by time) as id, time, week_close, week_high, week_low, week_gain, prev_week_gain, prev2_week_gain, week_volume, pre_week_volume, week4_volume, class from day_stat_data_hs300;

select time, gain, sum(gain) over (order by time) from (
select v.time, CASE WHEN (c.result=1) THEN abs(d.open-d.close) ELSE -abs(d.open-d.close) END as gain from day_data_classify c, day_data_stat_view v, day_data d where c.id=v.id and d.time::text=v.time) l;

create table hs300_gain as
select time, gain, sum(gain) over (order by time) from (
select v.time, CASE WHEN (c.result=1) THEN abs(d.open-d.close) ELSE -abs(d.open-d.close) END as gain from day_data_classify c, day_data_stat_view_hs300 v, day_data_hs300 d where c.id=v.id and d.time::text=v.time) l;

create view day_data_stat_view as select row_number() over (order by time) as id, time, week_close, week_high, week_low, week_gain, prev_week_gain, prev2_week_gain, week_volume, pre_week_volume, week4_volume, class from day_stat_data;
 
create view day_data_stat_view_hs300 as select row_number() over (order by time) as id, time, week_close, week_high, week_low, week_gain, prev_week_gain, prev2_week_gain, week_volume, pre_week_volume, week4_volume, class from day_stat_data_hs300;

create view day_data_stat_regress_view_hs300 as select row_number() over (order by time) as id, time, week_close, week_high, week_low, week_gain, prev_week_gain, prev2_week_gain, week_volume, pre_week_volume, week4_volume, class from day_data_stat_regress_hs300;

create table day_data_gain as
select time, gain, sum(gain) over (order by time),close from (
select v.time, CASE WHEN (c.result=1) THEN abs(d.open-d.close) ELSE -abs(d.open-d.close) END as gain, close from day_data_classify c, day_data_stat_view v, day_data d where c.id=v.id and d.time::text=v.time) l;
 
create table hs300_gain as
select time, gain, sum(gain) over (order by time), close from (
select v.time, CASE WHEN (c.result=1) THEN abs(d.open-d.close) ELSE -abs(d.open-d.close) END as gain, close from day_data_classify c, day_data_stat_view_hs300 v, day_data_hs300 d where c.id=v.id and d.time::text=v.time) l;


create table hs300_regress_gain as
select time, gain,gain2, sum(CASE when (gain<-80) then -80 ELSE gain END) over (order by time) as acc_gain, 
sum(CASE when (gain<-120) then -120 ELSE gain END) over (order by time) as acc_gain_2,
sum(CASE when (gain2<-80) then -80 ELSE gain2 END) over (order by time) as acc_gain2, close from (
    select v.time, 
    CASE WHEN (c.pred>0) THEN 
        d.close-d.open 
    ELSE 
        CASE WHEN (c.pred<-0) THEN
            d.open-d.close
        ELSE
            0
        END
    END as gain,
    CASE WHEN (c.pred>2) THEN 
        c.actual 
    ELSE 
        CASE WHEN (c.pred<-2) THEN
            -c.actual    
        ELSE
            0
        END
    END as gain2,
    d.close 
    from pred_hs300 c, day_data_stat_regress_view_hs300 v, day_data_hs300 d where c.id=v.id and d.time::text=v.time) l;

create table hs300_regress_gain as
select time, gain,gain2, sum(CASE when (gain<-open*0.03) then -open*0.03 ELSE gain END) over (order by time) as acc_gain, 
sum(CASE when (gain2<-open*0.03) then -open*0.03 ELSE gain2 END) over (order by time) as acc_gain2, close from (
    select v.time, 
    CASE WHEN (c.pred>0) THEN 
        d.close-d.open 
    ELSE 
        CASE WHEN (c.pred<-0) THEN
            d.open-d.close
        ELSE
            0
        END
    END as gain,
    CASE WHEN (c.pred>2) THEN 
        c.actual 
    ELSE 
        CASE WHEN (c.pred<-2) THEN
            -c.actual    
        ELSE
            0
        END
    END as gain2,
    d.close, d.open 
    from pred_hs300 c, day_data_stat_regress_view_hs300 v, day_data_hs300 d where c.id=v.id and d.time::text=v.time) l;


CREATE OR REPLACE FUNCTION slide_window_test
    ( 
    original_table_name     TEXT,
    window_size             INT,
    confidence              INT,
    max_depth               INT,
    out_table               TEXT
    ) 
RETURNS FLOAT8 AS $$
DECLARE
    index   BIGINT;
    size    BIGINT;
    stmt    TEXT;
    train_table   TEXT := 'temp_train';
    test_table    TEXT := 'temp_test';
    score   FLOAT8;
    num     INT := 0;
    acc_score   FLOAT8 :=0;
BEGIN
    EXECUTE 'SELECT max(id) from '||original_table_name INTO size;

    EXECUTE 'DROP TABLE IF EXISTS '||out_table||';';

    EXECUTE 'CREATE TABLE '||out_table||' (
                id  INT,
                result INT
                )';

    FOR index IN window_size..size LOOP
        
        EXECUTE 'DROP TABLE IF EXISTS '||train_table||';';
        stmt= 'CREATE TEMP TABLE '||train_table||' AS 
            SELECT * FROM 
            '||original_table_name||' WHERE id<'||index||' AND id>='||index-window_size;
        RAISE INFO '%',stmt;
        EXECUTE stmt;
    
        EXECUTE 'DROP TABLE IF EXISTS '||test_table||';';
        stmt= 'CREATE TEMP TABLE '||test_table||' AS 
            SELECT * FROM 
            '||original_table_name||' WHERE id='||index;
        RAISE INFO '%',stmt;
        EXECUTE stmt;
        
        PERFORM madlib.c45_train('infogain', train_table, 'madlib.temp_result', NULL,
            'week_close, week_high, week_low, week_gain, prev_week_gain, prev2_week_gain, week_volume, pre_week_volume,week4_volume',
            'week_close, week_high, week_low, week_gain, prev_week_gain, prev2_week_gain, week_volume, pre_week_volume,week4_volume', 
            'id', 'class', confidence, 'explicit', max_depth, 0.001, 0.01, 0);
        score = madlib.c45_score('madlib.temp_result', test_table, 0);
        
        IF (score > 0.0001) THEN
            EXECUTE 'INSERT INTO '||out_table||' values('||index||', 1)';
        ELSE
            EXECUTE 'INSERT INTO '||out_table||' values('||index||', 0)';
        END IF;

        acc_score = acc_score + score;
        num = num+1;
        RAISE INFO 'index: %, acc_score:%, num:%, ratio;%', index, acc_score, num, acc_score/num;
        PERFORM madlib.c45_clean('madlib.temp_result');
    END LOOP;
    RETURN acc_score/num;
END
$$ LANGUAGE PLPGSQL;

CREATE OR REPLACE FUNCTION slide_window_test_rf
    ( 
    original_table_name     TEXT,
    window_size             INT,
    tree_num                INT,
    max_depth               INT,
    out_table               TEXT
    ) 
RETURNS FLOAT8 AS $$
DECLARE
    index   BIGINT;
    size    BIGINT;
    stmt    TEXT;
    train_table   TEXT := 'temp_train';
    test_table    TEXT := 'temp_test';
    score   FLOAT8;
    num     INT := 0;
    acc_score   FLOAT8 :=0;
BEGIN
    EXECUTE 'SELECT max(id) from '||original_table_name INTO size;

    EXECUTE 'DROP TABLE IF EXISTS '||out_table||';';

    EXECUTE 'CREATE TABLE '||out_table||' (
                id  INT,
                result INT
                )';
    FOR index IN window_size..size LOOP
        
        EXECUTE 'DROP TABLE IF EXISTS '||train_table||';';
        stmt= 'CREATE TEMP TABLE '||train_table||' AS 
            SELECT * FROM 
            '||original_table_name||' WHERE id<'||index||' AND id>='||index-window_size;
        RAISE INFO '%',stmt;
        EXECUTE stmt;
    
        EXECUTE 'DROP TABLE IF EXISTS '||test_table||';';
        stmt= 'CREATE TEMP TABLE '||test_table||' AS 
            SELECT * FROM 
            '||original_table_name||' WHERE id='||index;
        RAISE INFO '%',stmt;
        EXECUTE stmt;
        
        PERFORM madlib.rf_train('infogain', train_table, 'madlib.temp_result', tree_num, NULL, 1, 
            'week_close, week_high, week_low, week_gain, prev_week_gain, prev2_week_gain, week_volume, pre_week_volume,week4_volume',
            'week_close, week_high, week_low, week_gain, prev_week_gain, prev2_week_gain, week_volume, pre_week_volume,week4_volume',
            'id', 'class', 'explicit', max_depth, 0.001, 0.01, 0);
        score = madlib.rf_score('madlib.temp_result', test_table, 0);

        acc_score = acc_score + score;
        num = num+1;
        IF (score > 0.0001) THEN
            EXECUTE 'INSERT INTO '||out_table||' values('||index||', 1)';
        ELSE
            EXECUTE 'INSERT INTO '||out_table||' values('||index||', 0)';
        END IF;
        RAISE INFO 'index: %, acc_score:%, num:%, ratio;%', index, acc_score, num, acc_score/num;
        PERFORM madlib.rf_clean('madlib.temp_result');
    END LOOP;
    RETURN acc_score/num;
END
$$ LANGUAGE PLPGSQL;

CREATE OR REPLACE FUNCTION generate_regress_stat_data
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
         close-open AS class
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
    EXECUTE 'DROP TABLE IF EXISTS temp_history_tr_table cascade';
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
    EXECUTE 'DROP TABLE IF EXISTS '||target_table_name||' cascade;';
    stmt= 'CREATE TABLE '||target_table_name||' AS 
        SELECT row_number() over (order by trans_time) as id, 
        trans_time, close, open/close as open, high/close as high, low/close as low, volume, open_interest, tr, 
    avg(volume) over 
        (ORDER BY trans_time ROWS BETWEEN 5 PRECEDING AND CURRENT ROW) as avg_volume_5,
    avg(volume) over 
        (ORDER BY trans_time ROWS BETWEEN 10 PRECEDING AND CURRENT ROW) as avg_volume_10,
    avg(volume) over 
        (ORDER BY trans_time ROWS BETWEEN 20 PRECEDING AND CURRENT ROW) as avg_volume_20,
    avg(open_interest) over 
        (ORDER BY trans_time ROWS BETWEEN 5 PRECEDING AND CURRENT ROW) as avg_open_interest_5,
    avg(open_interest) over 
        (ORDER BY trans_time ROWS BETWEEN 10 PRECEDING AND CURRENT ROW) as avg_open_interest_10,
    avg(open_interest) over 
        (ORDER BY trans_time ROWS BETWEEN 20 PRECEDING AND CURRENT ROW) as avg_open_interest_20,
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
        (ORDER BY trans_time ROWS BETWEEN 1 FOLLOWING AND 1 FOLLOWING) as avg_close_pred
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

select generate_history_stat_data('minute_data', 'minute_stat_data', 1.5);

create table profit_minute as
select id, sum(CASE WHEN (class='Hold') THEN 
                            0 
                       ELSE 
                            CASE WHEN(class = 'Sell') THEN
                                close-nxt_close
                            ELSE
                                -close+nxt_close
                            END
                       END) over (order by trans_time), trans_time 
from
(select n.id, n.trans_time, m.close, lead(m.close) over (order by n.trans_time) nxt_close, t.result class , t.res 
 from minute_data m, minute_stat_data n, minute_classify t where n.id=t.id and n.trans_time = m.trans_time) l order by trans_time;
