import java.nio.channels.FileChannel;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * Benchmarker
 */
public class Benchmarker{

    private static Benchmarker instance;

    private FileChannel source;
    private FileChannel dest;

    private long startTime = 0;
    private long elapsedTime = 0;

    public static enum BuffType {
        Heap,
        Direct
    }

    private Benchmarker() {
        System.out.println("New Benchmarker Singleton instance returned!");
    }
    
    static {
        try {
            instance = new Benchmarker();
        } catch (Exception e) {
            e.printStackTrace();
        } 
    }

    public static Benchmarker getInstance() {
        return instance; // Create new object only if it does not exist
    }

    public void runBufferedStream(String filename, BufferedInputStream fin, BufferedOutputStream fout) throws IOException{
        startTime();

        int i;
        while ((i = fin.read()) != -1) {
            fout.write(i);
        }
        
        stopTime();
        printStats("Copied " + filename + " successfully with BufferedStream");
    }

    public void runCustomBufferStream(String filename, FileInputStream fin, FileOutputStream fout) throws IOException{
        startTime();

        byte[] buffer = new byte[1024*8];
        int len;
        while((len = fin.read(buffer, 0, 1024)) > 0){
            fout.write(buffer, 0, len);
        }

        stopTime();
        printStats("Copied " + filename + " successfully with 8KB CustomBufferStream");
    }

    public void runTransferTo(String filename) throws IOException{
        startTime();

        long count = this.source.size();
        this.source.transferTo(0, count, this.dest);

        stopTime();
        printStats("Copied " + filename + " successfully with TransferTo");
        resetChannels();
    }

    public void runBuffer(String filename, int buffSize, BuffType type) throws IOException{
        if (!this.source.isOpen() || !this.dest.isOpen())
            new Throwable("Channels are not opened");

        startTime();
        buffSize = 1024 * buffSize;
        ByteBuffer buffer = null;
        switch (type) {
            case Heap:
                buffer = ByteBuffer.allocate(buffSize);
                break;
            case Direct:
                buffer = ByteBuffer.allocateDirect(buffSize);
                break;
            default:
                new Throwable("Invalid buffer type passed");
                break;
        }

        while (this.source.read(buffer) != -1) {
            buffer.flip();
            while (buffer.hasRemaining()) { dest.write (buffer); }
            buffer.clear();
        }

        stopTime();
        printStats("Copied " + filename + " successfully with " + Util.bytesToKilobytes(buffSize) + "KB " + type.toString() + "Buffer");
        resetChannels();
    }

    public void setChannels(FileChannel source, FileChannel dest){
        this.source = source;
        this.dest = dest;
    }

    private void resetChannels() throws IOException{
        this.source.position(0);
        this.dest.position(0);
    }

    private void startTime(){
        this.startTime = System.nanoTime();
    }

    private void stopTime(){
        long stopTime = System.nanoTime();
        this.elapsedTime = stopTime - this.startTime;
    }

    private void printStats(String res){
        System.out.println();
        System.out.println(res);
        System.out.println(Util.ConsoleColors.GREEN + "Elapsed time : " + this.elapsedTime + "ns [" + this.elapsedTime/1000000 + "ms]" + Util.ConsoleColors.RESET);
        System.out.println();
    }

}