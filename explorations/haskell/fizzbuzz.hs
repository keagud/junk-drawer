import System.Environment
import Text.Read

fizzbuzz :: Int -> String
fizzbuzz n = case (n `mod` 3, n `mod` 5) of
  (0, 0) -> "FizzBuzz"
  (0, _) -> "Fizz"
  (_, 0) -> "Buzz"
  _ -> show n

fizzBuzzRange :: Int -> [String]
fizzBuzzRange n = map ((++ "\n") . show . fizzbuzz) [1 .. n + 1]

argparse :: [String] -> Maybe Int
argparse [] = Nothing
argparse args = readMaybe (head args)

main = do
  args <- getArgs

  case argparse args of
    Nothing -> putStrLn "Error"
    -- I make no claim to understand this line
    Just n -> foldr ((>>) . putStrLn) (return ()) (fizzBuzzRange n)

    -- so it maps the composed function ((>>) . putStrLn) over the range.
    -- >> being more or less the comma operator from C and friends
    -- the (return ()) bit i don't get though. Initial value for foldr?
    -- Then why doesn't foldl (return ()) ((>>) . putStrLn) work??
