import System.Environment
import System.Process
import System.IO

process :: String -> String -> IO Handle
process "a" fname = do 
  x <- openFile fname AppendMode
  return x
process "w" fname = do
  x <- openFile fname WriteMode
  return x
process "c" cmd   = do
  x <- createProcess (shell cmd) {std_in = CreatePipe}
  let (Just ihandle, _, _, _) = x in
    return ihandle
printHelp = do
  putStrLn "Usage: bifurcate type targets...\n\
           \Types:\n\
           \  -a  Open the specified file(s) for appending.\n\
           \  -w  Open the specified file(s) for writing\n\
           \      (WARNING: will overwrite file contents).\n\
           \  -c  Run the specified commands.\n\
           \  -h  Print this help message.\n\
           \\n\
           \Note: targets corresponds to possible inputs."

collect :: String -> [String] -> [IO Handle]
collect t [] = []
collect t (('-' : hd) : tl) = parseArgs (('-' : hd) : tl)
collect t (hd : tl) = ((process t hd) : (collect t tl))

parseArgs :: [String] -> [IO Handle]
parseArgs [] = []
parseArgs (('-': hd) : tl) = collect hd tl

multicast :: [IO Handle] -> String -> IO ()
multicast [] c = do
  return ()
multicast (x : xs) c = do
  hdl <- x
  hPutStr hdl c
  hFlush hdl
  multicast xs c

bifurcate :: [String] -> String -> IO ()
bifurcate args c
  | (any (\ x -> x == "-h" || x == "--help") args) = do
    printHelp
    return ()
  | otherwise = multicast (parseArgs args) c

main :: IO ()
main = do
  args <- getArgs
  c <- getContents
  bifurcate args c
