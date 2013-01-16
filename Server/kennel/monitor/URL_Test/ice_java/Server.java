import Ice.*;

//ICE服务类
public class Server extends Ice.Application{

	//加载服务
	//本例中有二个，为HttpClient HttpParser
	//对应config.server中相应的服务名
	public int run(String[] arg0) {
		Ice.ObjectAdapter adapter = communicator().createObjectAdapter("HttpClient.Server");           
        adapter.add(new HttpClientI(), communicator().stringToIdentity("HttpClient"));
        adapter.add(new HttpParserI(), communicator().stringToIdentity("HttpParser"));
        adapter.activate();
        communicator().waitForShutdown();
        return 0;
	} 


    public static void main(String[] args)
    {
    	Server app = new Server();
        int status = app.main("Server",args,"config.server");
        if (status != 0)
        {
            System.exit(status); 
        }
    }

}
