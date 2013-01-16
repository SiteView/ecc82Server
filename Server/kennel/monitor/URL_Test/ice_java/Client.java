import site.*;
import Ice.*;

public class Client extends Application{

	public int run(String[] arg0) {
        HttpClinetBeanPrx hcp = HttpClinetBeanPrxHelper.checkedCast(communicator().propertyToProxy("HttpClient.Proxy").ice_twoway().ice_timeout(-1).ice_secure(false));
        HttpParserPrx hpp = HttpParserPrxHelper.checkedCast(communicator().propertyToProxy("HttpParser.Proxy").ice_twoway().ice_timeout(-1).ice_secure(false));
        hcp.init("220.249.96.131", 80, "http");
		//第一步
        String html = hcp.getHTML("/");
		String[] links = hpp.getLink(html);
		String[] forms = hpp.getForm(html);
		String[] inputs = hpp.getFormInput(html, 1);
		
		//第二步
         html = hcp.getHTML("/projects/ecc");
		 links = hpp.getLink(html);
		 forms = hpp.getForm(html);
		 inputs = hpp.getFormInput(html, 1);
		
		//第3步		
		inputs[2]+="bin.liu";
		inputs[3]+="binliu";
		String path = hcp.login(inputs, "/projects/ecc/login");
		
		//第4步
         html = hcp.getHTML(path);
		 links = hpp.getLink(html);
		 forms = hpp.getForm(html);
		 inputs = hpp.getFormInput(html, 1);
		 
			//第5步
         html = hcp.getHTML("/projects/ecc");
		 links = hpp.getLink(html);
		 forms = hpp.getForm(html);
		 inputs = hpp.getFormInput(html, 1);
		
		return 0;
	}

    public static void main(String[] args)
    {
        Client app = new Client();
        int status = app.main("Client", args, "config.client");
        System.exit(status);
    }
}
