package weka_test;
import weka.classifiers.Evaluation;
import java.sql.DriverManager;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.*;
import weka.classifiers.trees.*;
import weka.classifiers.functions.*;
import weka.core.Instances;
import weka.experiment.InstanceQuery;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import weka.core.Instances;
import weka.classifiers.meta.*;
import weka.classifiers.*;

public class MinuteAvgStatRegAnalyze {

	/**
	 * @param args
	 */

		public static void main(String[] args) {
			// TODO Auto-generated method stub
            System.out.println("-------- PostgreSQL JDBC Connection Testing ------------");
            try {
                    Class.forName("org.postgresql.Driver");
            } catch (ClassNotFoundException e) {
                    System.out.println("Where is your PostgreSQL JDBC Driver? "
                                    + "Include in your library path!");
                    e.printStackTrace();
                    return;
            }
            System.out.println("PostgreSQL JDBC Driver Registered!");
            Connection connection = null;
            Statement stmt = null;
            CallableStatement call_stmt = null;
            try {
                    connection = DriverManager.getConnection(
                                    "jdbc:postgresql://127.0.0.1:5432/testdb", "postgres",
                                    "123456");
                    
            } catch (SQLException e) {
                    System.out.println("Connection Failed! Check output console");
                    e.printStackTrace();
                    return;
            }
            if (connection != null) {
                    System.out.println("You made it, take control your database now!");
            } else {
                    System.out.println("Failed to make connection!");
            }

            int start_index = 10;
            
            
            try {
            	stmt = connection.createStatement();
            	stmt.executeUpdate("drop table if exists minute_classify;");
            	stmt.executeUpdate("create table minute_classify(id integer, res integer, result float8);");
            	call_stmt = connection.prepareCall("{call generate_minute_stat_reg_data('minute_data', 'minute_stat_data',"+start_index+",0,1)}");
            	call_stmt.executeUpdate();
            	call_stmt.close();
            	//stmt.executeUpdate("select generate_history_stat_data('minute_data', 'minute_stat_data',0.5);");
            }
            catch (SQLException e)
            {
            	e.printStackTrace();
            }
            
			try{
			    double correct=0;
			    double total_variance=0;
			    double mean_variance=0;
			    int num=0;
			    int window=50;
			    int index =0;
				 InstanceQuery query = new InstanceQuery();
				 query.setUsername("postgres");
				 query.setPassword("123456");		    
				System.out.println("start OK");
				String out_file = "e:\\stock_analyzer\\minute_data_classify.txt";
				String csv_file = "e:\\stock_analyzer\\minute_data_classify.csv";
				String csv_file2 = "e:\\stock_analyzer\\minute_data_classify2.csv";
				String csv_file3 = "e:\\stock_analyzer\\minute_data_classify3.csv";
				String csv_file4 = "e:\\stock_analyzer\\minute_data_classify4.csv";
		        BufferedWriter writer = new BufferedWriter(
		                     new FileWriter(out_file));
				query.setQuery("select stochastic_K, stochastic_D, slow_stochastic_D, "
						+ "Momentum_1, Momentum_2, ROC, Williams_R, AD_Oscillator, Disparity_5, Disparity_10, OSCP,"
						+ "CCI, tr, atr_5, atr_10, "
						+ "prev_class1, prev_class2, prev_class3, prev_class4, prev_class5, "
						+ "prev_class6, prev_class7, prev_class8, prev_class9, prev_class10, "
						+ "class "
						+ "from minute_stat_data minute_stat_data order by trans_time");
		        
				Instances full_set = query.retrieveInstances();
				for(index=start_index; index<= 171228-window-10;index++)
				//for(index=start_index; index<= 575-window-10; index++)
				{

				 
				 System.out.println("set query complete");
				 Instances training_data = new Instances(full_set, index, window);
				 if( training_data.numInstances() !=window)
				 {
					 return ;
				 }
				 System.out.println("get training set complete");

				 int test_index = index+window;
				 Instances test_data  = new Instances(full_set, test_index, 1);
				 System.out.println("get test set complete");
				 if( test_data.numInstances()!=1)
				 {
					 return;
				 }			 
				 if (training_data.classIndex() == -1)
					 training_data.setClassIndex(training_data.numAttributes() - 1);	
				 if (test_data.classIndex() == -1)
					 test_data.setClassIndex(test_data.numAttributes() - 1);	
				 Instances labeled = new Instances(test_data);
				 System.out.println("setClassIndex");
				 //AdaBoostM1 tree = new AdaBoostM1();         // new instance of tree
				 //M5P tree = new M5P();
				 Classifier tree = new AdditiveRegression();
				 //SMOreg tree = new SMOreg();
				 String disable_pruning="-R";
				 String[] options=new String[2];
				 options[0]= "-S 4 -K 2";
				 options[1]= "-C 78";
				 //options[2]= "-g 0.04";
				 //tree.setOptions(weka.core.Utils.splitOptions("-C 78.0 -L 0.0010 -P 1.0E-12 -N 0 -K \"weka.classifiers.functions.supportVector.RBFKernel -C 250007 -G 25\""));

				 //tree.setOptions(weka.core.Utils.splitOptions("-C 1.0 -L 0.0010 -P 1.0E-12 -N 0 -V -1 -W 1 -K \"weka.classifiers.functions.supportVector.PolyKernel -C 250007 -E 1.0\""));
				 tree.setOptions(weka.core.Utils.splitOptions("-S 0.1 -I 100 -W weka.classifiers.trees.REPTree -- -M 2 -V 0.001 -N 3 -S 1 -L 4 -P"));
				 //tree.setOptions(weka.core.Utils.splitOptions("-s 1 -t 2"));
				 //tree.setOptions(weka.core.Utils.splitOptions("-W weka.classifiers.functions.LibSVM -I 5"));
				 //tree.setOptions(options);
				 System.out.println("create random forest");
				 try
				 {
				 tree.buildClassifier(training_data);   // build classifier
				 }
				 catch(Exception e)
				 {
					 e.printStackTrace();
					 continue;
				 }
				 System.out.println("build model OK");
	/*			 Evaluation eval = new Evaluation(training_data);
				 System.out.println("before evaluation");
				 //eval.crossValidateModel(tree, training_data, 10, new Random(1));		
				 eval.evaluateModel(tree, training_data);
				 System.out.println(eval.toSummaryString("\nResults\n======\n", false));	
				 correct+= eval.correct();
				 num++;
				 System.out.println("eval complete");*/
				 // label instances
				 for (int i = 0; i < test_data.numInstances(); i++) {
				   double clsLabel = tree.classifyInstance(test_data.instance(i));

				   double origLabel = test_data.instance(i).classValue();

				   total_variance += Math.abs(clsLabel-origLabel);
				   if(clsLabel*origLabel>0.0)
				   {
					   correct++;
				   }
				   writer.write(test_index+1+",1,"+clsLabel);
				   writer.newLine();
				   num++;
				   mean_variance = total_variance/num;
				   System.out.println("Num:"+num+" mean_variance:"+Double.toString(mean_variance));
				 }		
				 System.out.println("classify complete");
				 System.out.println("Num:"+num+" mean_variance:"+Double.toString(mean_variance));

			    /*}
			    System.out.println("Correct ratio:"+Double.toString(correct/num));*/
				}
				 query.close();
				 writer.close();
				 stmt.executeUpdate("copy minute_classify from '"+out_file+"' delimiter ',';");
				 System.out.println("end OK");
				 System.out.println("Num:"+num+" Correct ratio:"+Double.toString(correct/num));
				 
				 stmt.executeUpdate("drop table if exists profit_minute");
		         call_stmt = connection.prepareCall("{call Calc_Profit_Reg(5, 0.2, 'profit_minute')}");
	             call_stmt.executeUpdate();
	             call_stmt.close();
				 //stmt.executeUpdate("select Calc_Profit();");
				 stmt.executeUpdate("copy profit_minute to '"+csv_file+"' delimiter ',';");

		         call_stmt = connection.prepareCall("{call Calc_Profit_Reg(10, 0.2, 'profit_minute')}");
	             call_stmt.executeUpdate();
	             call_stmt.close();
				 //stmt.executeUpdate("select Calc_Profit();");
				 stmt.executeUpdate("copy profit_minute to '"+csv_file2+"' delimiter ',';");		

		         call_stmt = connection.prepareCall("{call Calc_Profit_Reg(20, 0.2, 'profit_minute')}");
	             call_stmt.executeUpdate();
	             call_stmt.close();
				 //stmt.executeUpdate("select Calc_Profit();");
				 stmt.executeUpdate("copy profit_minute to '"+csv_file3+"' delimiter ',';");	
				 
		         call_stmt = connection.prepareCall("{call Calc_Profit_Reg(30, 0.2, 'profit_minute')}");
	             call_stmt.executeUpdate();
	             call_stmt.close();
				 //stmt.executeUpdate("select Calc_Profit();");
				 stmt.executeUpdate("copy profit_minute to '"+csv_file4+"' delimiter ',';");	
				 
				 stmt.close();
			}
			catch(Exception e)
			{
				e.printStackTrace();
			}
		}


}

