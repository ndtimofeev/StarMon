{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE TypeOperators #-}
{-# LANGUAGE DataKinds #-}
{-# LANGUAGE StandaloneDeriving #-}
{-# LANGUAGE FlexibleInstances #-}

module Main where

-- base 
import Prelude hiding ( putStrLn )

import GHC.Generics

import System.Environment

-- QuicCheck
import Test.QuickCheck.Gen

-- aeson
import Data.Aeson

-- bytestring
import Data.ByteString.Lazy

--optparse-generic
import Options.Generic

-- internal
import StarGen

data GenMod = SquareSimple | EllipseSimple
    deriving (Show, Read, Generic, ParseRecord, ParseField, ParseFields)

data StarGenCmd w
    = Square
        { number       :: w ::: Int <!> "1000" <#> "n" <?> "Number generated stars"
        , width        :: w ::: Int <!> "500"  <#> "W" <?> "Width of star box"
        , height       :: w ::: Int <!> "500"  <#> "H" <?> "Height of star box"
        , maxhyperline :: w ::: Int <!> "0" <?> "Max hyperline num for one star"
        , stargenmode  :: w ::: GenMod <?> "Select genmod type" } 
    deriving Generic

instance ParseRecord (StarGenCmd Wrapped)
deriving instance Show (StarGenCmd Unwrapped)

main :: IO ()
main = do
    x   <- unwrapRecord "TEST" 
    box <- case stargenmode x of
        SquareSimple  -> generate $ starBoxGenSimple (number x) (maxhyperline x) (0, 0) (width x, height x)
        EllipseSimple -> generate $ starBoxGenSimple2 (number x) (maxhyperline x) (0, 0) (width x, height x)
    putStrLn $ encode box
