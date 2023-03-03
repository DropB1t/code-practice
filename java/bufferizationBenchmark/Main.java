import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.FileAlreadyExistsException;
import java.io.RandomAccessFile;
import java.nio.channels.FileChannel;
import java.nio.file.Files;
import java.nio.file.Paths;

/**
 * Main
 * 
 *  I risultati ripostati si trovano in: output_1.txt & output_2.txt
 * 
 */
public class Main {

    private static final String in_folder = "./input";
    private static final int[] Sizes = {8, 16, 32, 64};

    public static void main(String[] args) {

        if (args.length != 1) {
            System.out.println("Specify a file upon which execute benchmark tests");
            System.out.println("Or specify --test to test on the whole folder with assignment's files");
            return;
        }

        Benchmarker bench = Benchmarker.getInstance();

        try {
            Files.createDirectory(Paths.get("output"));
        } catch (FileAlreadyExistsException e) {
        } catch (IOException e) {
            Util.printException(e);
        }

        switch (args[0]) {
            case "--test":
                File folder = new File(in_folder);
                if (!folder.exists() || !folder.isDirectory()) {
                    System.err.println("No such file or directory: " + in_folder);
                    return;
                }
                File[] files = folder.listFiles();
                for (File file : files) {
                    runTests(file,bench);
                }
                break;
            default:
                File file = new File(args[0]);
                if (!file.exists() || !file.isFile()) {
                    System.err.println("No such file: " + args[0]);
                    return;
                }
                runTests(file, bench);
                break;
        }

        return;
    }

    public static void runTests(File file, Benchmarker bench){
        if (file.isFile()){
            String outputFile = "./output/copy_"+file.getName();
            try (FileChannel source = new RandomAccessFile(file,"r").getChannel(); FileChannel dest = new RandomAccessFile(outputFile,"rw").getChannel()) {
                bench.setChannels(source,dest);
                
                for (int size : Sizes)
                    bench.runBuffer(file.getName(), size, Benchmarker.BuffType.Direct);

                for (int size : Sizes)
                    bench.runBuffer(file.getName(), size, Benchmarker.BuffType.Heap);

                bench.runTransferTo(file.getName());
            } catch (Exception e) {
                Util.printException(e);
            }

            try (BufferedInputStream fin = new BufferedInputStream(new FileInputStream(file));BufferedOutputStream fout = new BufferedOutputStream(new FileOutputStream(outputFile))) {
                bench.runBufferedStream(file.getName(), fin, fout);
            } catch (Exception e) {
                Util.printException(e);
            }

            try (FileInputStream fin = new FileInputStream(file);FileOutputStream fout = new FileOutputStream(outputFile)) {
                bench.runCustomBufferStream(file.getName(), fin, fout);
            } catch (Exception e) {
                Util.printException(e);
            }
        }
    }
}