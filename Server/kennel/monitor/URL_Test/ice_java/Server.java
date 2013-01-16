import Ice.*;

//ICE������
public class Server extends Ice.Application{

	//���ط���
	//�������ж�����ΪHttpClient HttpParser
	//��Ӧconfig.server����Ӧ�ķ�����
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
