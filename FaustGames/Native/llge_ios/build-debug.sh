xcodebuild -project llge_ios.xcodeproj -target llge_ios -sdk iphoneos -arch arm64 -configuration Debug clean build
cp build/Debug-iphoneos/libllge_ios.a libllge_ios-arm64.a
xcodebuild -project llge_ios.xcodeproj -target llge_ios -sdk iphoneos -arch armv7 -configuration Debug clean build
cp build/Debug-iphoneos/libllge_ios.a libllge_ios-armv7.a
lipo -create -output libllge_ios.a libllge_ios-arm64.a libllge_ios-armv7.a


