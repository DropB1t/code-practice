import java.util.concurrent.ExecutorService;
import java.util.zip.GZIPOutputStream;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;
import java.io.FileOutputStream;
import java.util.stream.Stream;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.util.List;
import java.io.File;

/**
 * Zipper
 * Data una lista di directories scadisce ciascuna di esse (ricorsivamente) e comprime tutti i file utilizzando l'utility gzip.
 * Non sappiamo quanto sono annidate le sottocartelle, ho munito ciascuno FolderScanner con un suo CachedThreadPool per 
 * creare più parallelismo possibile. I task FolderScanner sono sottoscritti a un FixedThreadPool per non esagerare 
 * con allocazione di troppi Thread. Testato con una cartella da 80MB ~ Terminazione della compressione in circa 6sec
 */
public class Zipper {

    private static void printException(Exception e) {
        System.out.println("toString(): " + e.toString());
        System.out.println("getMessage(): " + e.getMessage());
        System.out.println("StackTrace: ");
        e.printStackTrace();
    }

    private static void service_shutdown(ExecutorService service){
        try {
            service.shutdown();
            service.awaitTermination(32, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            printException(e);
        } finally {
            service.shutdownNow();
        }
    }

    private static class FolderScanner implements Runnable {
        private String dir;
        private ExecutorService service;

        FolderScanner(String dir, ExecutorService service) {
            this.dir = dir;
            this.service = service;
        }

        @Override
        public void run() {
            List<Path> pathList = new ArrayList<>();
            try (Stream<Path> stream = Files.walk(Paths.get(dir))) {
                pathList = stream.map(Path::normalize)
                        .filter(Files::isRegularFile)
                        .collect(Collectors.toList());
            } catch (IOException e) {
                printException(e);
            }
            pathList.forEach((path) -> service.execute(new FileZipper(path)));
            service_shutdown(service);
        }
    }

    private static class FileZipper implements Runnable {
        private Path file;
        private Path zippedfile;

        FileZipper(Path file) {
            this.file = file;
            zippedfile = Paths.get((file.toString() + ".gz"));
        }

        @Override
        public void run() {
            //System.out.println("Try to zip " + file.toString() + " into " + zippedfile.toString());
            try {
                GZIPOutputStream gos = new GZIPOutputStream(new FileOutputStream(zippedfile.toFile()));
                FileInputStream fis = new FileInputStream(file.toFile());
                byte[] buffer = new byte[1024];
                int len;
                while ((len = fis.read(buffer)) > 0) {
                    gos.write(buffer, 0, len);
                }
                gos.close();
                fis.close();
            } catch (IOException e) {
                printException(e);
            }
            //System.out.println(Thread.currentThread().getName() + " has finished");
        }
    }

    public static void main(String[] args) {
        ExecutorService folder_service = Executors.newFixedThreadPool(8);
        ExecutorService files_service;
        long startTime = System.currentTimeMillis();
        for (String path : args) {
            File file = new File(path);
            if (file.exists() && file.isDirectory()) {
                files_service = Executors.newCachedThreadPool();
                folder_service.execute(new FolderScanner(path, files_service));
            }
        }
        service_shutdown(folder_service);
        System.out.printf("Completed all tasks in %d ms\n", System.currentTimeMillis() - startTime);
    }
}
