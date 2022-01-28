module StarGen where

-- base
import Data.List ( sort )
import GHC.Generics

-- aeson
import Data.Aeson

-- unordered-containers
import Data.HashSet ( empty, insert, member, toList )

-- KdTree
import Data.Trees.KdTree ( Point(..) )

-- QuickCheck
import Test.QuickCheck.Arbitrary
import Test.QuickCheck.Gen

instance Point Star where
    dimension _ = 2
    coord n p   = case n of { 0 -> fromIntegral (x p); 1 -> fromIntegral (y p); _ -> undefined }
    dist2 p0 p1 = dist p0 p1 

data Hyperline = Hyperline { name1 :: String, name2 :: String }
    deriving (Ord, Show, Generic, ToJSON)

instance Eq Hyperline where
    h1 == h2 = (name1 h1 == name1 h2 && name2 h1 == name2 h2) || (name1 h1 == name2 h2 && name2 h1 == name1 h2)

data StarBox = StarBox
    { stars :: [Star], hyperlines :: [Hyperline] }
    deriving (Eq, Ord, Show, Generic, ToJSON)

data Color = Red | Blue | Yellow | Orange | White
    deriving (Eq, Ord, Show, Generic, ToJSON, Enum, Bounded)

data Star = Star { name :: String, color :: Color, x :: !Int, y :: !Int }
    deriving (Eq, Ord, Show, Generic, ToJSON)

dist :: Star -> Star -> Double
dist s1 s2 = sqrt $ fromIntegral ((x s1 - x s2) ^ 2 + (y s1 - y s2) ^ 2)

naiveGenColor :: Gen Color
naiveGenColor = elements [minBound .. maxBound]

namesGen :: Int -> Gen [(Char, Int)]
namesGen n = toList <$> go n empty
    where
        go i acc 
            | i <= 0    = pure acc
            | otherwise = do
                n <- nameGen' `suchThat` (\x -> not (member x acc))
                go (i - 1) (insert n acc)

starGensDiv :: Int -> (Int, Int) -> (Int, Int) -> Gen [Star]
starGensDiv n pt0 pt1 = do
    names <- namesGen n
    undefined 

nameGen' :: Gen (Char, Int)
nameGen' = do
    l <- choose ('A', 'Z')
    n <- choose (10, 900)
    pure (l, n)

nameGen :: Gen String
nameGen = do
    l <- choose ('A', 'Z')
    n <- choose (10, 900 :: Int)
    pure (l : show n)

nearst :: Star -> [Star] -> [(Double, Star)]
nearst st ss = sort $ map (\s -> (dist st s, s)) ss

hyperinesGen2 :: Int -> [Star] -> Gen [Hyperline]
hyperinesGen2 max stars
    | max <= 0  = pure []
    | otherwise = go stars []
    where
        go [] xs       = pure xs
        go (s : ss) xs = do
            hls <- hyperinesGen max stars xs s
            go ss (xs ++ hls)

hyperinesGen :: Int -> [Star] -> [Hyperline] -> Star -> Gen [Hyperline]
hyperinesGen max stars lines start = do
    num <- choose (0, max)
    go (num - cnt :: Int) []
    where
        cnt             = foldl (\acc v -> acc + if name1 v == name start || name2 v == name start then 1 else 0) 0 lines
        nst             = take 25 $ nearst start stars
        hyperine        = do
            (_, s) <- elements nst
            pure (Hyperline (name start) (name s))

        go n acc
            | n <= 0    = pure acc
            | otherwise = do
                x <- hyperine `suchThat` (`notElem` acc ++ lines)
                go (n - 1) (x : acc)


starBoxGenSimple2 :: Int -> Int -> (Int, Int) -> (Int, Int) -> Gen StarBox
starBoxGenSimple2 n max p1 p2 = do
    stars  <- starGens2 n p1 p2
    hlines <- hyperinesGen2 max stars
    pure $ StarBox stars hlines

starBoxGenSimple :: Int -> Int -> (Int, Int) -> (Int, Int) -> Gen StarBox
starBoxGenSimple n max p1 p2 = do
    stars  <- starGens n p1 p2
    hlines <- hyperinesGen2 max stars
    pure $ StarBox stars hlines

starGens2 :: Int -> (Int, Int) -> (Int, Int) -> Gen [Star]
starGens2 n bottomLeft upperRight = go [] n
    where
        go acc n
            | n <= 0    = pure acc
            | otherwise = do
                s <- starGen (ellipseGen bottomLeft upperRight) `suchThat` (\s -> all (\t -> name s /= name t && (x s, y s) /= (x t, y t)) acc)
                go (s : acc) (n - 1)

starGens :: Int -> (Int, Int) -> (Int, Int) -> Gen [Star]
starGens n bottomLeft upperRight = go [] n
    where
        go acc n
            | n <= 0    = pure acc
            | otherwise = do
                s <- starGen (boxGen bottomLeft upperRight) `suchThat` (\s -> all (\t -> name s /= name t && (x s, y s) /= (x t, y t)) acc)
                go (s : acc) (n - 1)


-- x ^ 2 / a ^ 2 + y ^ 2 / b ^ 2 <= 1 
ellipseGen :: (Int, Int) -> (Int, Int) -> Gen (Int, Int)
ellipseGen (x0, y0) (x1, y1) = do
    x <- choose (x0, x1) 
    let a  = fromIntegral (x1 - x0) / 2
        b  = fromIntegral (y1 - y0) / 2
        x' = fromIntegral x - a
        y' = sqrt (((a ** 2) * (b ** 2) - (b ** 2) * (x' ** 2)) / (a ** 2))
    y <- choose (round (b - y'), round (y' + b))
    pure (x, y) 

boxGen :: (Int, Int) -> (Int, Int) -> Gen (Int, Int)
boxGen (x0, y0) (x1, y1) = do
    x <- choose (x0, x1)
    y <- choose (y0, y1)
    pure (x, y)

starGen :: Gen (Int, Int) -> Gen Star
starGen genPos = do
    txt <- nameGen
    pos <- genPos
    c   <- naiveGenColor
    pure $ uncurry (Star txt c) pos
