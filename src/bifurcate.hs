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

collect :: String -> [String] -> [IO Handle]
collect t [] = []
collect t (('-' : hd) : tl) = parseArgs (hd : tl)
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

main :: IO ()
main = do
  args <- getArgs
  c <- getContents
  multicast (parseArgs args) c
