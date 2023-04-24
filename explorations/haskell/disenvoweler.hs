import Data.Char
import Data.List
import System.Environment

-- this is the most i've worked for such a basic program

strToUpper = map toUpper

strToLower = map toLower

doubleCases s = do
  let upper = strToUpper s
  let lower = strToLower s
  lower ++ upper

vowels = doubleCases "aeiou"

isInList :: Char -> [Char] -> Bool
isInList c ls = case (c, ls) of
 (c, []) -> False
 (c, ls) -> head ls == c || isInList c (tail ls)

isVowel :: Char -> Bool
isVowel c = isInList c vowels

disenvowel :: String -> String
disenvowel s = [c | c <- s, not (isVowel c)]

main = do
  args <- getArgs

  if null args
    then putStrLn "No input"
    else putStrLn (disenvowel (head args))
